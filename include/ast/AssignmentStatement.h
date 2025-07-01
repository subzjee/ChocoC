#pragma once

#include "ast/ASTNode.h"
#include "ast/Expression.h"
#include "ast/SimpleStatement.h"
#include "ast/Target.h"

#include "llvm/ADT/ArrayRef.h"

#include <memory>
#include <vector>

namespace chocopy::ast {
class AssignmentStatement : public SimpleStatement {
public:
  AssignmentStatement(const std::vector<Target>& targets,
                      std::unique_ptr<Expression> expr)
      : m_targets(targets), m_expr(std::move(expr)) {};

  /// Get all targets to be assigned to.
  /// @returns The targets.
  [[nodiscard]] llvm::ArrayRef<Target> getTargets() const {
    return m_targets;
  }

  /// Get the expression on the right-hand side.
  /// @returns The expression.
  [[nodiscard]] const std::unique_ptr<Expression>& getExpr() const {
    return m_expr;
  }

  [[nodiscard]] llvm::SMRange getLocation() const override {
    return {m_targets.front().getLocation().Start, m_expr->getLocation().End};
  }

  std::any accept(ASTVisitor& visitor) const override;

private:
  const std::vector<Target> m_targets;
  const std::unique_ptr<Expression> m_expr;
};
} // namespace chocopy::ast