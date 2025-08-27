#include "parser/Parser.h"
#include "ast/BinaryExpression.h"
#include "ast/Expression.h"
#include "ast/GroupingExpression.h"
#include "ast/Identifier.h"
#include "ast/Literal.h"
#include "ast/UnaryExpression.h"

using namespace llvm;

namespace chocopy::parser {
[[nodiscard]] BindingPower getPrefixPower(TokenType op) {
  switch (op) {
  case TokenType::NOT:
    return {{40, 40}};
  case TokenType::MINUS:
    return {{80, 80}};
  default:
    return std::nullopt;
  }
}

[[nodiscard]] BindingPower getInfixPower(TokenType op) {
  switch (op) {
  case TokenType::OR:
    return {{20, 21}};
  case TokenType::AND:
    return {{30, 31}};
  case TokenType::EQUAL:
    [[fallthrough]];
  case TokenType::NEQUAL:
    [[fallthrough]];
  case TokenType::LESS:
    [[fallthrough]];
  case TokenType::GREAT:
    [[fallthrough]];
  case TokenType::LESSEQ:
    [[fallthrough]];
  case TokenType::GREATEQ:
    [[fallthrough]];
  case TokenType::IS:
    return {{50, 50}};
  case TokenType::PLUS:
    [[fallthrough]];
  case TokenType::MINUS:
    return {{60, 61}};
  case TokenType::MULT:
    [[fallthrough]];
  case TokenType::DIV:
    [[fallthrough]];
  case TokenType::MOD:
    return {{70, 71}};
  default:
    return std::nullopt;
  }
}

std::unique_ptr<ast::Program> Parser::parseProgram() {
  ProgramChildren children{};

  // Check if it's an empty program. If so, return immediately.
  if (!m_token_stream.peek()) {
    return std::make_unique<ast::Program>(children);
  }

  while (m_token_stream.peek()) {
    if (match(TokenType::ID) && m_token_stream.peek(1) == TokenType::COLON) {
      auto var_def_ctx = parseVariableDefinition();
      if (!var_def_ctx) {
        return nullptr;
      }

      children.push_back(std::move(var_def_ctx));
      continue;
    }

    break;
  }

  while (m_token_stream.peek()) {
    auto stmt_ctx = parseStatement();
    if (!stmt_ctx) {
      return nullptr;
    }

    children.push_back(std::move(stmt_ctx));
  }

  return std::make_unique<ast::Program>(children);
}

std::unique_ptr<ast::Type> Parser::parseType() {
  if (expect(TokenType::ID, TokenType::IDSTRING)) {
    return std::make_unique<ast::Type>(*m_token_stream.peek(-1));
  } else if (expect(TokenType::OPENBRACK)) {
    auto type_ctx = parseType();

    if (!type_ctx) {
      return nullptr;
    }

    if (!expect(TokenType::CLOSEBRACK)) {
      m_diag_manager.report(DiagID::ExpectedToken,
                            m_token_stream.peek()->get().getLocation(), {"]"});
      return nullptr;
    }

    return std::make_unique<ast::Type>(type_ctx->getBaseType(),
                                       type_ctx->getDimension() + 1);
  }

  m_diag_manager.report(DiagID::ExpectedToken,
                        m_token_stream.peek()->get().getLocation(), {"a type"});

  return nullptr;
}

std::unique_ptr<ast::TypedVariable> Parser::parseTypedVariable() {
  if (!expect(TokenType::ID)) [[unlikely]] {
    m_diag_manager.report(DiagID::ExpectedToken,
                          m_token_stream.peek()->get().getLocation(),
                          {"an identifier"});
    return nullptr;
  }

  std::unique_ptr<ast::Identifier> identifier =
      std::make_unique<ast::Identifier>(*m_token_stream.peek(-1));

  if (!expect(TokenType::COLON)) [[unlikely]] {
    m_diag_manager.report(DiagID::ExpectedToken,
                          m_token_stream.peek()->get().getLocation(), {":"});
    return nullptr;
  }

  auto type_ctx = parseType();
  if (!type_ctx) {
    return nullptr;
  }

  return std::make_unique<ast::TypedVariable>(std::move(identifier),
                                              std::move(type_ctx));
}

std::unique_ptr<ast::VariableDefinition> Parser::parseVariableDefinition() {
  auto typed_var_ctx = parseTypedVariable();
  if (!typed_var_ctx) {
    return nullptr;
  }

  if (!expect(TokenType::ASSIGN)) {
    m_diag_manager.report(DiagID::ExpectedToken,
                          m_token_stream.peek()->get().getLocation(), {"="});
    return nullptr;
  }

  auto literal_ctx = parseLiteral();
  if (!literal_ctx) {
    return nullptr;
  }

  if (!expect(TokenType::NEWLINE)) {
    m_diag_manager.report(DiagID::ExpectedToken,
                          m_token_stream.peek(-1)->get().getLocation(),
                          {"a new line after"});
    return nullptr;
  }

  return std::make_unique<ast::VariableDefinition>(std::move(typed_var_ctx),
                                                   std::move(literal_ctx));
}

std::unique_ptr<ast::Literal> Parser::parseLiteral() {
  if (!expect(TokenType::NONE, TokenType::FALSE, TokenType::TRUE,
              TokenType::INTLIT, TokenType::IDSTRING, TokenType::STRING)) {
    m_diag_manager.report(DiagID::ExpectedToken,
                          m_token_stream.peek()->get().getLocation(),
                          {"a literal value"});
    return nullptr;
  }

  return std::make_unique<ast::Literal>(*m_token_stream.peek(-1));
}

[[nodiscard]] std::unique_ptr<ast::Expression> Parser::parseTarget() {
  if (!expect(TokenType::ID)) {
    m_diag_manager.report(DiagID::ExpectedToken,
                          m_token_stream.peek(-1)->get().getLocation(),
                          {"an identifier"});
    return nullptr;
  }

  return std::make_unique<ast::Identifier>(*m_token_stream.peek(-1));
}

[[nodiscard]] std::unique_ptr<ast::Expression>
Parser::parseExpression(unsigned int min_power) {
  auto lhs = parsePrefix();

  if (!lhs) {
    return nullptr;
  }

  while (const auto token = m_token_stream.peek()) {
    const auto token_type = token->get().getType();

    if (!token->get().isBinOp()) {
      break;
    }

    const auto binding_power = getInfixPower(token_type);

    if (!binding_power || binding_power->first < min_power) {
      break;
    }

    m_token_stream.advance();

    auto rhs = parseExpression(binding_power->second);

    if (!rhs) {
      return nullptr;
    }

    // `AND` and `OR` are the only binary operators that have expressions
    // instead of constant expressions as their operands. All constant
    // expressions are expressions but not vice versa. Therefore, if the
    // operator is either of these, we don't have to check whether the operands
    // are constant expressions.
    if (token_type == TokenType::AND || token_type == TokenType::OR) {
      lhs = std::make_unique<ast::BinaryExpression>(
          std::move(lhs), token->get(), std::move(rhs));
      continue;
    }

    if (!lhs->isConstantExpression()) {
      m_diag_manager.report(DiagID::ExpectedToken, lhs.get()->getLocation(),
                            {"a constant expression"});
      return nullptr;
    }

    if (!rhs->isConstantExpression()) {
      m_diag_manager.report(DiagID::ExpectedToken, rhs.get()->getLocation(),
                            {"a constant expression"});
      return nullptr;
    }

    lhs = std::make_unique<ast::BinaryExpression>(
        std::move(lhs),
        token->get(),
        std::move(rhs));
  }

  return lhs;
}

[[nodiscard]] std::unique_ptr<ast::Statement> Parser::parseStatement() {
  if (match(TokenType::IF)) { // TODO: If-elif-else statements
    return nullptr;
  }
  
  if (match(TokenType::WHILE)) { // TODO: While statements
    return nullptr;
  }

  if (match(TokenType::FOR)) { // TODO: For statements
    return nullptr;
  }

  if (match(TokenType::PASS, TokenType::RETURN, TokenType::ID,
                   TokenType::NOT, TokenType::TRUE, TokenType::FALSE,
                   TokenType::NONE, TokenType::INTLIT, TokenType::IDSTRING,
                   TokenType::STRING, TokenType::OPENBRACK,
                   TokenType::OPENPAREN, TokenType::MINUS)) {
    auto simple_stmt = parseSimpleStatement();
    if (!simple_stmt) {
      return nullptr;
    }

    if (!expect(TokenType::NEWLINE)) {
      m_diag_manager.report(DiagID::ExpectedToken,
                            m_token_stream.peek(-1)->get().getLocation(),
                            {"a new line after"});
      return nullptr;
    }

    return simple_stmt;
  }

  m_diag_manager.report(DiagID::ExpectedToken,
                        m_token_stream.peek()->get().getLocation(),
                        {"a statement"});

  return nullptr;
}

[[nodiscard]] std::unique_ptr<ast::Statement>
Parser::parseSimpleStatement() {
  if (m_token_stream.peek(1) == TokenType::ASSIGN) {
    return parseAssignmentStatement();
  }

  return nullptr;
}

[[nodiscard]] std::unique_ptr<ast::AssignmentStatement>
Parser::parseAssignmentStatement() {
  std::vector<std::unique_ptr<ast::Expression>> targets{};

  while (match(TokenType::ID) && m_token_stream.peek(1) == TokenType::ASSIGN) {
    auto target = parseTarget();
    if (!target) {
      return nullptr;
    }

    targets.push_back(std::move(target));

    if (!expect(TokenType::ASSIGN)) {
      m_diag_manager.report(DiagID::ExpectedToken,
                            m_token_stream.peek()->get().getLocation(), {"="});
      return nullptr;
    }
  }

  auto expr_ctx = parseExpression();
  if (!expr_ctx) {
    return nullptr;
  }

  return std::make_unique<ast::AssignmentStatement>(std::move(targets),
                                                    std::move(expr_ctx));
}

[[nodiscard]] std::unique_ptr<ast::Expression> Parser::parsePrefix() {
  const auto token = m_token_stream.peek();

  if (!token) {
    return nullptr;
  }

  if (expect(TokenType::STRING, TokenType::IDSTRING, TokenType::FALSE,
             TokenType::TRUE, TokenType::INTLIT, TokenType::NONE)) {
    return std::make_unique<ast::Literal>(*token);
  } 

  if (expect(TokenType::ID)) {
    return std::make_unique<ast::Identifier>(*token);
  }
  
  if (expect(TokenType::OPENPAREN)) {
    auto left_paren = m_token_stream.peek(-1);
    auto expr = parseExpression(0);

    if (!expect(TokenType::CLOSEPAREN)) {
      m_diag_manager.report(DiagID::UnclosedParenthesis,
                            token->get().getLocation());
      return nullptr;
    }
    auto right_paren = m_token_stream.peek(-1);

    return std::make_unique<ast::GroupingExpression>(
        left_paren->get(), std::move(expr), right_paren->get());
  }

  
  if (expect(TokenType::NOT, TokenType::MINUS)) {
    const auto& op = m_token_stream.peek(-1);
    auto rhs = parseExpression(getPrefixPower(op->get().getType())->second);

    if (!rhs || (op->get().getType() == TokenType::MINUS && !rhs->isConstantExpression())) {
      m_diag_manager.report(DiagID::MissingUnaryOperand,
                            op->get().getLocation(), {op->get().getText().str()});
      return nullptr;
    }

    return std::make_unique<ast::UnaryExpression>(
        op->get(), std::move(rhs));
  }

  return nullptr;
}
} // namespace chocopy