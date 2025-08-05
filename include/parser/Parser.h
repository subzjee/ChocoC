#pragma once

#include "TokenStream.h"
#include "ast/AssignmentStatement.h"
#include "ast/Expression.h"
#include "ast/Literal.h"
#include "ast/Program.h"
#include "ast/Statement.h"
#include "ast/TypedVariable.h"
#include "ast/VariableDefinition.h"
#include "diagnostics/DiagnosticsManager.h"
#include "parser/ExpressionParser.h"

namespace chocopy {
class Parser {
public:
  Parser(TokenStream& token_stream, DiagnosticsManager& diagnostics_manager)
      : m_expression_parser{token_stream, diagnostics_manager},
        m_token_stream(token_stream), m_diag_manager(diagnostics_manager) {};

  [[nodiscard]] std::unique_ptr<ast::Program> parse() {
    return parseProgram();
  };

  /// Parse a program.
  /// @returns AST node for a program.
  [[nodiscard]] std::unique_ptr<ast::Program> parseProgram();

  /// Parse a typed variable.
  /// @returns AST node for a typed variable.
  [[nodiscard]] std::unique_ptr<ast::TypedVariable> parseTypedVariable();

  /// Parse a type.
  /// @returns AST node for a type.
  [[nodiscard]] std::unique_ptr<ast::Type> parseType();

  /// Parse a variable definition.
  /// @returns AST node for a variable definition.
  [[nodiscard]] std::unique_ptr<ast::VariableDefinition>
  parseVariableDefinition();

  /// Parse a literal.
  /// @returns AST node for a literal.
  [[nodiscard]] std::unique_ptr<ast::Literal> parseLiteral();

  /// Parse a target.
  /// @returns AST node for a target.
  [[nodiscard]] std::unique_ptr<ast::Expression> parseTarget();

  /// Parse an expression.
  /// @returns AST node for an expression.
  [[nodiscard]] std::unique_ptr<ast::Expression> parseExpression();

  /// Parse a statement.
  /// @returns AST node for a statement.
  [[nodiscard]] std::unique_ptr<ast::Statement> parseStatement();

  /// Parse a simple statement.
  /// @returns AST node for a simple statement.
  [[nodiscard]] std::unique_ptr<ast::Statement> parseSimpleStatement();

  /// Parse an assignment statement.
  /// @returns AST node for an assignment statement.
  [[nodiscard]] std::unique_ptr<ast::AssignmentStatement>
  parseAssignmentStatement();

private:
  /// Check whether the current token in the stream matches one of the given token types.
  /// @returns Whether current token matches.
  template <typename... TokenTypes>
  [[nodiscard]] bool match(const TokenTypes&... token_types) {
    const auto current_token = m_token_stream.peek();

    if (!current_token) {
      return false;
    }

    if (((current_token->get().getType() == token_types) || ...)) {
      return true;
    }

    return false;
  }

  /// Check whether the current token in the stream matches one of the given token types.
  /// If so, advance the stream.
  /// @returns Whether the current token matches.
  template <typename... TokenTypes>
  [[nodiscard]] bool expect(const TokenTypes&... token_types) {
    if (!match(token_types...)) {
      return false;
    }

    m_token_stream.advance();

    return true;
  }

  ExpressionParser m_expression_parser;
  TokenStream& m_token_stream;
  DiagnosticsManager& m_diag_manager;
};
} // namespace chocopy