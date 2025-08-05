#include "parser/Parser.h"
#include "ast/Expression.h"

using namespace llvm;

namespace chocopy {
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

[[nodiscard]] std::unique_ptr<ast::Expression> Parser::parseExpression() {
  return m_expression_parser.parseExpression();
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
} // namespace chocopy