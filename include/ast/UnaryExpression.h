#pragma once

#include "ast/Expression.h"
#include "lexer/Token.h"

#include <memory>
#include <type_traits>

namespace chocopy::ast {

class UnaryExpression : public Expression {
public:
  UnaryExpression(const Token& op, std::unique_ptr<Expression> rhs)
      : Expression(/*is_constant_expression*/ op.getType() == TokenType::MINUS), m_op(op), m_rhs(std::move(rhs)) {
#ifndef NDEBUG
    assert(op.isUnaryOp());
    if (op.getType() == TokenType::MINUS) {
      assert(m_rhs->isConstantExpression());
    }
#endif
  };

  [[nodiscard]] const Token& getOperator() const { return m_op; }
  [[nodiscard]] const std::unique_ptr<Expression>& getRHS() const {
    return m_rhs;
  };

  [[nodiscard]] llvm::SMRange getLocation() const override {
    return {m_op.getLocation().Start, m_rhs->getLocation().End};
  }

  std::any accept(ASTVisitor& visitor) const override;

private:
  const Token& m_op;
  std::unique_ptr<Expression> m_rhs;
};
} // namespace chocopy::ast