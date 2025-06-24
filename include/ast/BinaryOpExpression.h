#pragma once

#include "ast/ConstantExpression.h"
#include "lexer/Token.h"

#include <memory>

namespace chocopy::ast {
class BinaryOpExpression : public ConstantExpression {
public:
  BinaryOpExpression(std::unique_ptr<ConstantExpression>& lhs, const Token& op,
                     std::unique_ptr<ConstantExpression>& rhs)
      : m_lhs(std::move(lhs)), m_op(op), m_rhs(std::move(rhs)) {};

  [[nodiscard]] std::unique_ptr<ConstantExpression>& getLHS() { return m_lhs; };
  [[nodiscard]] const Token& getOperator() const { return m_op; }
  [[nodiscard]] std::unique_ptr<ConstantExpression>& getRHS() { return m_rhs; };

  [[nodiscard]] llvm::SMRange getLocation() const override {
    return {m_lhs->getLocation().Start, m_rhs->getLocation().End};
  }

private:
  std::unique_ptr<ConstantExpression> m_lhs;
  const Token& m_op;
  std::unique_ptr<ConstantExpression> m_rhs;
};
} // namespace chocopy::ast