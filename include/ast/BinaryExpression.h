#pragma once

#include "ast/ASTNode.h"
#include "ast/Expression.h"
#include "lexer/Token.h"

#include <memory>

namespace chocopy::ast {

class BinaryExpression : public Expression {
public:
  BinaryExpression(std::unique_ptr<Expression> lhs, const Token& op,
                   std::unique_ptr<Expression> rhs)
      : Expression(NK_BinaryExpression, /*is_cexpr*/ op.isBinOp() && op.getType() != TokenType::AND &&
             op.getType() != TokenType::OR), m_lhs(std::move(lhs)), m_op(op), m_rhs(std::move(rhs)) {
#ifndef NDEBUG
    assert(op.isBinOp());
    if (op.getType() != TokenType::AND && op.getType() != TokenType::OR) {
      assert(m_lhs->isConstantExpression() && m_rhs->isConstantExpression());
    }
#endif
  };

  /// Get the left-hand side expression.
  /// @returns The left-hand side expression.
  [[nodiscard]] const std::unique_ptr<Expression>& getLHS() const {
    return m_lhs;
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
    return {m_lhs->getLocation().Start, m_rhs->getLocation().End};
  }

  std::any accept(ASTVisitor& visitor) const override;

  /// Check whether \p node is a BinaryExpression.
  /// @returns Whether \p node is a BinaryExpression.
  static bool classof(const ASTNode* node);

private:
  std::unique_ptr<Expression> m_lhs;
  const Token& m_op;
  std::unique_ptr<Expression> m_rhs;
};
} // namespace chocopy::ast