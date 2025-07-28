#pragma once

#include "DiagnosticsManager.h"
#include "ast/ASTVisitor.h"
#include "ast/AssignmentStatement.h"
#include "ast/GroupingExpression.h"
#include "ast/Identifier.h"
#include "ast/Literal.h"
#include "ast/VariableDefinition.h"
#include "semantic/TypeEnvironment.h"

namespace chocopy {
class TypeChecker : public ASTVisitor {
public:
  using ASTVisitor::visit;

  TypeChecker(SymbolTable& symbol_table,
              DiagnosticsManager& diagnostics_manager)
      : m_diag_manager{diagnostics_manager}, m_local_env(symbol_table) {};

  std::any visit(const ast::VariableDefinition& ctx) override;
  std::any visit(const ast::AssignmentStatement& ctx) override;
  std::any visit(const ast::Literal& ctx) override;
  std::any visit(const ast::Identifier& ctx) override;
  std::any visit(const ast::BinaryExpression<ast::Expression>& ctx) override;
  std::any visit(const ast::BinaryExpression<ast::ConstantExpression>& ctx) override;
  std::any visit(const ast::GroupingExpression& ctx) override;

private:
  DiagnosticsManager& m_diag_manager;
  TypeEnvironment m_local_env;
};
} // namespace chocopy