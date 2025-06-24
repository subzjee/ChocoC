#pragma once

#include "ast/Literal.h"
#include "ast/WithLocation.h"

#include "llvm/Support/SourceMgr.h"

#include <memory>

namespace chocopy::ast {

class ConstantExpression : public WithLocation {
  using CExpr = std::variant<std::unique_ptr<Literal>>;

public:
  ConstantExpression(CExpr&& cexpr) : m_cexpr(std::move(cexpr)) {};

  /// Get the location.
  /// @returns The location.
  [[nodiscard]] llvm::SMRange getLocation() const override {
    return std::visit(
        [](const auto& cexpr) -> const llvm::SMRange {
          return cexpr->getLocation();
        },
        m_cexpr);
  };

  template <typename Visitor> auto visit(Visitor&& visitor) const {
    return std::visit(std::forward<Visitor>(visitor), m_cexpr);
  }

private:
  const CExpr m_cexpr;
};
} // namespace chocopy::ast