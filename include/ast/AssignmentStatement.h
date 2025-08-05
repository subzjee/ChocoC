#pragma once

#include "ast/ASTNode.h"
#include "ast/Expression.h"
#include "ast/Statement.h"

#include "llvm/ADT/ArrayRef.h"

#include <memory>
#include <vector>

namespace chocopy::ast {
class AssignmentStatement : public Statement {
public:
  AssignmentStatement(std::vector<std::unique_ptr<Expression>> targets,
                      std::unique_ptr<Expression> expr)
      : Statement(NK_AssignmentStatement), m_targets(std::move(targets)), m_expr(std::move(expr)) {
        #ifndef NDEBUG
          for (const auto& target : m_targets) {
            assert(target->isValidTarget() && "Expression is not a valid target");
          }
        #endif
      };

  /// Get all targets to be assigned to.
  /// @returns The targets.
  [[nodiscard]] llvm::ArrayRef<std::unique_ptr<Expression>> getTargets() const { return m_targets; }

  /// Get the expression on the right-hand side.
  /// @returns The expression.
  [[nodiscard]] const std::unique_ptr<Expression>& getExpr() const {
    return m_expr;
  }

  [[nodiscard]] llvm::SMRange getLocation() const override {
    return {m_targets.front()->getLocation().Start, m_expr->getLocation().End};
  }

  std::any accept(ASTVisitor& visitor) const override;

  /// Check whether \p node is an AssignmentStatement.
  /// @returns Whether \p node is an AssignmentStatement.
  static bool classof(const ASTNode* node);

private:
  const std::vector<std::unique_ptr<Expression>> m_targets;
  const std::unique_ptr<Expression> m_expr;
};
} // namespace chocopy::ast