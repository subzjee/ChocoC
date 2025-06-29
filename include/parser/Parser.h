#pragma once

#include "TokenStream.h"
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

namespace chocopy {
class Parser {
public:
  Parser(TokenStream& token_stream, DiagnosticsManager& diagnostics_manager)
      : m_token_stream(token_stream), m_diag_manager(diagnostics_manager) {};

  [[nodiscard]] std::unique_ptr<ast::Program> parse() {
    return parseProgram();
  };

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

private:
  TokenStream& m_token_stream;
  DiagnosticsManager& m_diag_manager;
};
} // namespace chocopy