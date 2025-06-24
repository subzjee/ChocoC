#pragma once

#include "ast/Expression.h"
#include "ast/Target.h"

#include "llvm/ADT/ArrayRef.h"

#include <memory>
#include <vector>

namespace chocopy::ast {
class AssignmentStatement {
public:
  AssignmentStatement(const std::vector<Target>& targets,
                      std::unique_ptr<Expression>& expr)
      : m_targets(targets), m_expr(std::move(expr)) {};

  /// Get all targets to be assigned to.
  /// @returns The targets.
  [[nodiscard]] const llvm::ArrayRef<Target> getTargets() const {
    return m_targets;
  }

  /// Get the expression on the right-hand side.
  /// @returns The expression.
  [[nodiscard]] const std::unique_ptr<Expression>& getExpr() const {
    return m_expr;
  }

private:
  const std::vector<Target> m_targets;
  const std::unique_ptr<Expression> m_expr;
};
} // namespace chocopy::ast