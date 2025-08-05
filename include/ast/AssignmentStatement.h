#pragma once

#include "ast/Expression.h"
#include "ast/Statement.h"
#include "ast/Target.h"

#include "llvm/ADT/ArrayRef.h"

#include <memory>
#include <vector>

namespace chocopy::ast {
class AssignmentStatement : public Statement {
public:
  AssignmentStatement(std::vector<std::unique_ptr<Target>> targets,
                      std::unique_ptr<Expression> expr)
      : Statement(StatementKind::SK_Assignment), m_targets(std::move(targets)), m_expr(std::move(expr)) {};

  /// Get all targets to be assigned to.
  /// @returns The targets.
  [[nodiscard]] llvm::ArrayRef<std::unique_ptr<Target>> getTargets() const { return m_targets; }

  /// Get the expression on the right-hand side.
  /// @returns The expression.
  [[nodiscard]] const std::unique_ptr<Expression>& getExpr() const {
    return m_expr;
  }

  [[nodiscard]] llvm::SMRange getLocation() const override {
    return {m_targets.front()->getLocation().Start, m_expr->getLocation().End};
  }

  std::any accept(ASTVisitor& visitor) const override;

  /// Check whether \p statement is an AssignmentStatement.
  /// @returns Whether \p statement is an AssignmentStatement.
  static bool classof(const Statement* statement) { return statement->getKind() == StatementKind::SK_Assignment; }

private:
  const std::vector<std::unique_ptr<Target>> m_targets;
  const std::unique_ptr<Expression> m_expr;
};
} // namespace chocopy::ast