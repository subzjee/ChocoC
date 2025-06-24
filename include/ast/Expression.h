#pragma once

#include "ast/ConstantExpression.h"
#include "ast/WithLocation.h"

#include <memory>

namespace chocopy::ast {
class Expression : public WithLocation {
  using Expr = std::variant<std::unique_ptr<ConstantExpression>>;

public:
  Expression(Expr&& expr) : m_expr(std::move(expr)) {};

  /// Get the location.
  /// @returns The location.
  [[nodiscard]] llvm::SMRange getLocation() const override {
    return std::visit(
        [](const auto& expr) -> const llvm::SMRange {
          return expr->getLocation();
        },
        m_expr);
  };

  template <typename Visitor> auto visit(Visitor&& visitor) const {
    return std::visit(std::forward<Visitor>(visitor), m_expr);
  }

private:
  const Expr m_expr;
};
} // namespace chocopy::ast