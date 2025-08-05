#pragma once

#include "ast/Expression.h"
#include "lexer/Token.h"

#include <memory>

namespace chocopy::ast {

class UnaryExpression : public Expression {
public:
  UnaryExpression(const Token& op, std::unique_ptr<Expression> rhs)
      : Expression(ExpressionKind::EK_UnaryExpression, /*is_cexpr*/ op.getType() == TokenType::MINUS), m_op(op), m_rhs(std::move(rhs)) {
#ifndef NDEBUG
    assert(op.isUnaryOp());
    if (op.getType() == TokenType::MINUS) {
      assert(m_rhs->isConstantExpression());
    }
#endif
  };

  /// Get the operator.
  /// @returns The operator.
  [[nodiscard]] const Token& getOperator() const { return m_op; }

  /// Get the right-hand side expression.
  /// @returns The right-hand side expression.
  [[nodiscard]] const std::unique_ptr<Expression>& getRHS() const {
    return m_rhs;
  };

  /// Get the source location.
  /// @returns The source location.
  [[nodiscard]] llvm::SMRange getLocation() const override {
    return {m_op.getLocation().Start, m_rhs->getLocation().End};
  }

  std::any accept(ASTVisitor& visitor) const override;

  /// Check whether \p expr is a UnaryExpression.
  /// @returns Whether \p expr is a UnaryExpression.
  static bool classof(const Expression* expr) {
    return expr->getKind() == ExpressionKind::EK_UnaryExpression;
  }

private:
  const Token& m_op;
  std::unique_ptr<Expression> m_rhs;
};
} // namespace chocopy::ast