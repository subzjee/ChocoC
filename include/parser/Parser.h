#pragma once

#include "DiagnosticsManager.h"
#include "ast/AssignmentStatement.h"
#include "ast/ConstantExpression.h"
#include "ast/Expression.h"
#include "ast/Literal.h"
#include "ast/Program.h"
#include "ast/SimpleStatement.h"
#include "ast/Statement.h"
#include "ast/Target.h"
#include "ast/TypedVariable.h"
#include "ast/VariableDefinition.h"
#include "lexer/Token.h"

namespace chocopy {
class Parser {
public:
  Parser(std::span<const Token> tokens, DiagnosticsManager& diagnostics_manager)
      : m_tokens(tokens), m_diag_manager(diagnostics_manager) {};

  [[nodiscard]] std::unique_ptr<ast::Program> parse() {
    return parseProgram();
  };

private:
  friend class ParserTest;

  [[nodiscard]] std::optional<std::reference_wrapper<const Token>>
  peek(const int n = 0) const;

  std::optional<std::reference_wrapper<const Token>> advance();

  template <typename... TokenTypes>
  [[nodiscard]] bool match(const TokenTypes&... token_types) {
    const auto current_token = peek();

    if (!current_token) {
      return false;
    }

    if (((current_token->get().getType() == token_types) || ...)) {
      advance();
      return true;
    }

    return false;
  }

  [[nodiscard]] std::unique_ptr<ast::Program> parseProgram();
  [[nodiscard]] std::unique_ptr<ast::TypedVariable> parseTypedVar();
  [[nodiscard]] std::unique_ptr<ast::Type> parseType();
  [[nodiscard]] std::unique_ptr<ast::VariableDefinition> parseVarDef();
  [[nodiscard]] std::unique_ptr<ast::Literal> parseLiteral();
  [[nodiscard]] std::unique_ptr<ast::Target> parseTarget();
  [[nodiscard]] std::unique_ptr<ast::Expression> parseExpr();
  [[nodiscard]] std::unique_ptr<ast::ConstantExpression> parseConstantExpr();
  [[nodiscard]] std::unique_ptr<ast::Statement> parseStmt();
  [[nodiscard]] std::unique_ptr<ast::SimpleStatement> parseSimpleStmt();
  [[nodiscard]] std::unique_ptr<ast::AssignmentStatement> parseAssignStmt();

  std::span<const Token> m_tokens;
  std::size_t m_current_idx{0};
  DiagnosticsManager& m_diag_manager;
};
} // namespace chocopy