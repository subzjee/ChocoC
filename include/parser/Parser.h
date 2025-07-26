#pragma once

#include "DiagnosticsManager.h"
#include "TokenStream.h"
#include "ast/AssignmentStatement.h"
#include "ast/Expression.h"
#include "ast/Literal.h"
#include "ast/Program.h"
#include "ast/SimpleStatement.h"
#include "ast/Statement.h"
#include "ast/Target.h"
#include "ast/TypedVariable.h"
#include "ast/VariableDefinition.h"
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

  [[nodiscard]] std::unique_ptr<ast::Program> parseProgram();
  [[nodiscard]] std::unique_ptr<ast::TypedVariable> parseTypedVariable();
  [[nodiscard]] std::unique_ptr<ast::Type> parseType();
  [[nodiscard]] std::unique_ptr<ast::VariableDefinition>
  parseVariableDefinition();
  [[nodiscard]] std::unique_ptr<ast::Literal> parseLiteral();
  [[nodiscard]] std::unique_ptr<ast::Target> parseTarget();
  [[nodiscard]] std::unique_ptr<ast::Expression> parseExpression();
  [[nodiscard]] std::unique_ptr<ast::Statement> parseStatement();
  [[nodiscard]] std::unique_ptr<ast::SimpleStatement> parseSimpleStatement();
  [[nodiscard]] std::unique_ptr<ast::AssignmentStatement>
  parseAssignmentStatement();

private:
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