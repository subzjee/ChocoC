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
std::unique_ptr<ast::Program> Parser::parseProgram() {
  ProgramChildren children{};

  // Check if it's an empty program. If so, return immediately.
  if (!m_token_stream.peek()) {
    return std::make_unique<ast::Program>(children);
  }

  while (m_token_stream.peek()) {
    if (m_token_stream.match(TokenType::ID) &&
        m_token_stream.peek(1) == TokenType::COLON) {
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
  if (m_token_stream.match(TokenType::ID, TokenType::IDSTRING)) {
    m_token_stream.advance();
    return std::make_unique<ast::Type>(*m_token_stream.peek(-1));
  } else if (m_token_stream.match(TokenType::OPENBRACK)) {
    m_token_stream.advance();
    auto type_ctx = parseType();

    if (!type_ctx) {
      return nullptr;
    }

    if (!m_token_stream.match(TokenType::CLOSEBRACK)) {
      m_diag_manager.addError(formatv("expected `]`"),
                              m_token_stream.peek()->get().getLocation());
      return nullptr;
    }
    m_token_stream.advance();

    return std::make_unique<ast::Type>(type_ctx->getBaseType(),
                                       type_ctx->getDimension() + 1);
  }

  m_diag_manager.addError(formatv("expected a type"),
                          m_token_stream.peek()->get().getLocation());

  return nullptr;
}

std::unique_ptr<ast::TypedVariable> Parser::parseTypedVariable() {
  if (!m_token_stream.match(TokenType::ID)) [[unlikely]] {
    m_diag_manager.addError(formatv("expected an identifier"),
                            m_token_stream.peek()->get().getLocation());
    return nullptr;
  }
  m_token_stream.advance();

  const auto &name = m_token_stream.peek(-1);

  if (!m_token_stream.match(TokenType::COLON)) [[unlikely]] {
    m_diag_manager.addError(formatv("expected `:`"),
                            m_token_stream.peek()->get().getLocation());
    return nullptr;
  }
  m_token_stream.advance();

  auto type_ctx = parseType();
  if (!type_ctx) {
    return nullptr;
  }

  return std::make_unique<ast::TypedVariable>(*name, std::move(type_ctx));
}

std::unique_ptr<ast::VariableDefinition> Parser::parseVariableDefinition() {
  auto typed_var_ctx = parseTypedVariable();
  if (!typed_var_ctx) {
    return nullptr;
  }

  if (!m_token_stream.match(TokenType::ASSIGN)) {
    m_diag_manager.addError(formatv("expected `=`"),
                            m_token_stream.peek()->get().getLocation());
    return nullptr;
  }
  m_token_stream.advance();

  auto literal_ctx = parseLiteral();
  if (!literal_ctx) {
    return nullptr;
  }

  if (!m_token_stream.match(TokenType::NEWLINE)) {
    const auto location = m_token_stream.peek(-1)->get().getLocation();
    m_diag_manager.addError(formatv("expected a new line"),
                            {location.End, location.End});
    return nullptr;
  }
  m_token_stream.advance();

  return std::make_unique<ast::VariableDefinition>(std::move(typed_var_ctx), std::move(literal_ctx));
}

std::unique_ptr<ast::Literal> Parser::parseLiteral() {
  if (!m_token_stream.match(TokenType::NONE, TokenType::FALSE, TokenType::TRUE,
                            TokenType::INTLIT, TokenType::IDSTRING,
                            TokenType::STRING)) {
    m_diag_manager.addError(formatv("expected a literal value"),
                            m_token_stream.peek()->get().getLocation());
    return nullptr;
  }
  m_token_stream.advance();

  return std::make_unique<ast::Literal>(*m_token_stream.peek(-1));
}

[[nodiscard]] std::unique_ptr<ast::Target> Parser::parseTarget() {
  if (m_token_stream.match(TokenType::ID)) {
    m_token_stream.advance();
    return std::make_unique<ast::Target>(*m_token_stream.peek(-1));
  }

  return nullptr;
}

[[nodiscard]] std::unique_ptr<ast::Expression> Parser::parseExpression() {
  return m_expression_parser.parseExpression();
}

[[nodiscard]] std::unique_ptr<ast::Statement> Parser::parseStatement() {
  if (m_token_stream.match(TokenType::IF)) {
    m_token_stream.advance();
    return nullptr;
  } else if (m_token_stream.match(TokenType::WHILE)) {
    m_token_stream.advance();
    return nullptr;
  } else if (m_token_stream.match(TokenType::FOR)) {
    m_token_stream.advance();
    return nullptr;
  } else if (auto simple_stmt = parseSimpleStatement()) {
    if (!m_token_stream.match(TokenType::NEWLINE)) {
      const auto location = m_token_stream.peek(-1)->get().getLocation();
      m_diag_manager.addError(formatv("expected a new line"),
                              {location.End, location.End});
      return nullptr;
    }
    m_token_stream.advance();

    return simple_stmt;
  }

  return nullptr;
}

[[nodiscard]] std::unique_ptr<ast::SimpleStatement>
Parser::parseSimpleStatement() {
  if (m_token_stream.peek(1) == TokenType::ASSIGN) {
    return parseAssignmentStatement();
  }

  return nullptr;
}

[[nodiscard]] std::unique_ptr<ast::AssignmentStatement>
Parser::parseAssignmentStatement() {
  std::vector<ast::Target> targets{};

  while (m_token_stream.peek(1) == TokenType::ASSIGN) {
    targets.push_back(std::move(*parseTarget()));
    m_token_stream.advance();
  }

  auto expr_ctx = parseExpression();

  if (!expr_ctx) {
    return nullptr;
  }

  return std::make_unique<ast::AssignmentStatement>(targets, std::move(expr_ctx));
}
} // namespace chocopy