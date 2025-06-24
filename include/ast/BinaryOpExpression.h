#pragma once

#include "ast/ConstantExpression.h"

#include <memory>

namespace chocopy::ast {
class BinaryOpExpression : public ConstantExpression {
public:
  BinaryOpExpression(std::unique_ptr<ConstantExpression>& lhs,
                     std::unique_ptr<ConstantExpression>& rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {};

  [[nodiscard]] llvm::SMRange getLocation() const override {
    return {m_lhs->getLocation().Start, m_rhs->getLocation().End};
  }

private:
  std::unique_ptr<ConstantExpression> m_lhs;
  std::unique_ptr<ConstantExpression> m_rhs;
};
} // namespace chocopy::ast