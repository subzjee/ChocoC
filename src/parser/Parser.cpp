#include "parser/Parser.h"

#include "ast/AssignmentStatement.h"
#include "ast/ConstantExpression.h"
#include "ast/Expression.h"
#include "ast/SimpleStatement.h"
#include "ast/TypedVariable.h"
#include "ast/VariableDefinition.h"
#include "lexer/Token.h"
#include "lexer/TokenType.h"

#include "llvm/Support/FormatVariadic.h"

using namespace llvm;

namespace chocopy {
std::optional<std::reference_wrapper<const Token>>
Parser::peek(const int n) const {
  const auto idx = m_current_idx + n;

  if (idx < 0 || idx >= m_tokens.size()) {
    return std::nullopt;
  }

  return std::cref(m_tokens[idx]);
}

std::optional<std::reference_wrapper<const Token>> Parser::advance() {
  const auto current_token = peek();

  if (current_token) {
    ++m_current_idx;
  }

  return current_token;
}

std::unique_ptr<ast::Program> Parser::parseProgram() {
  ProgramChildren children{};

  // Check if it's an empty program. If so, return immediately.
  if (!peek()) {
    return std::make_unique<ast::Program>(children);
  }

  while (peek()) {
    if (peek() == TokenType::ID && peek(1) == TokenType::COLON) {
      auto var_def_ctx = parseVarDef();

      if (!var_def_ctx) {
        return nullptr;
      }

      children.push_back(std::move(var_def_ctx));
      continue;
    }

    break;
  }

  while (peek()) {
    auto stmt_ctx = parseStmt();

    if (!stmt_ctx) {
      return nullptr;
    }

    children.push_back(std::move(stmt_ctx));
  }

  return std::make_unique<ast::Program>(children);
}

std::unique_ptr<ast::Type> Parser::parseType() {
  if (match(TokenType::ID, TokenType::IDSTRING)) {
    return std::make_unique<ast::Type>(*peek(-1));
  } else if (match(TokenType::OPENBRACK)) {
    auto type_ctx = parseType();

    if (!type_ctx) {
      return nullptr;
    }

    if (!match(TokenType::CLOSEBRACK)) {
      m_diag_manager.addError(formatv("expected `]`"),
                              peek()->get().getLocation());
      return nullptr;
    }

    return std::make_unique<ast::Type>(type_ctx->getBaseType(),
                                       type_ctx->getDimension() + 1);
  }

  m_diag_manager.addError(formatv("expected a type"),
                          peek()->get().getLocation());

  return nullptr;
}

std::unique_ptr<ast::TypedVariable> Parser::parseTypedVar() {
  if (!match(TokenType::ID)) [[unlikely]] {
    m_diag_manager.addError(formatv("expected an identifier"),
                            peek()->get().getLocation());
    return nullptr;
  }

  const auto& name = peek(-1);

  if (!match(TokenType::COLON)) [[unlikely]] {
    m_diag_manager.addError(formatv("expected `:`"),
                            peek()->get().getLocation());
    return nullptr;
  }

  auto type_ctx = parseType();
  if (!type_ctx) {
    return nullptr;
  }

  return std::make_unique<ast::TypedVariable>(*name, type_ctx);
}

std::unique_ptr<ast::VariableDefinition> Parser::parseVarDef() {
  auto typed_var_ctx = parseTypedVar();
  if (!typed_var_ctx) {
    return nullptr;
  }

  if (!match(TokenType::ASSIGN)) {
    m_diag_manager.addError(formatv("expected `=`"),
                            peek()->get().getLocation());
    return nullptr;
  }

  auto literal_ctx = parseLiteral();
  if (!literal_ctx) {
    return nullptr;
  }

  if (!match(TokenType::NEWLINE)) {
    const auto location = peek(-1)->get().getLocation();
    m_diag_manager.addError(formatv("expected a new line"),
                            {location.End, location.End});
    return nullptr;
  }

  return std::make_unique<ast::VariableDefinition>(typed_var_ctx, literal_ctx);
}

std::unique_ptr<ast::Literal> Parser::parseLiteral() {
  if (!match(TokenType::NONE, TokenType::FALSE, TokenType::TRUE,
             TokenType::INTLIT, TokenType::IDSTRING, TokenType::STRING)) {
    m_diag_manager.addError(formatv("expected a literal value"),
                            peek()->get().getLocation());
    return nullptr;
  }

  return std::make_unique<ast::Literal>(*peek(-1));
}

[[nodiscard]] std::unique_ptr<ast::Target> Parser::parseTarget() {
  if (match(TokenType::ID)) {
    return std::make_unique<ast::Target>(*peek(-1));
  }

  return nullptr;
}

[[nodiscard]] std::unique_ptr<ast::ConstantExpression>
Parser::parseConstantExpr() {
  if (peek()->get().isLiteral()) {
    return parseLiteral();
  }

  return nullptr;
}

[[nodiscard]] std::unique_ptr<ast::Expression> Parser::parseExpr() {
  if (peek()->get().isLiteral()) {
    return parseConstantExpr();
  }

  return nullptr;
}

[[nodiscard]] std::unique_ptr<ast::Statement> Parser::parseStmt() {
  if (match(TokenType::IF)) {
    return nullptr;
  } else if (match(TokenType::WHILE)) {
    return nullptr;
  } else if (match(TokenType::FOR)) {
    return nullptr;
  } else if (auto simple_stmt = parseSimpleStmt()) {
    if (!match(TokenType::NEWLINE)) {
      const auto location = peek(-1)->get().getLocation();
      m_diag_manager.addError(formatv("expected a new line"),
                              {location.End, location.End});
      return nullptr;
    }

    return simple_stmt;
  }

  m_diag_manager.addError(formatv("expected a statement"),
                          peek()->get().getLocation());
  return nullptr;
}

[[nodiscard]] std::unique_ptr<ast::SimpleStatement> Parser::parseSimpleStmt() {
  if (peek(1) == TokenType::ASSIGN) {
    return parseAssignStmt();
  }

  return nullptr;
}

[[nodiscard]] std::unique_ptr<ast::AssignmentStatement>
Parser::parseAssignStmt() {
  std::vector<ast::Target> targets{};

  while (peek(1) == TokenType::ASSIGN) {
    targets.push_back(std::move(*parseTarget()));
    advance();
  }

  auto expr_ctx = parseExpr();

  return std::make_unique<ast::AssignmentStatement>(targets, expr_ctx);
}
} // namespace chocopy