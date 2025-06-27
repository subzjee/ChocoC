#pragma once

#include "ast/Expression.h"

namespace chocopy::ast {
struct ConstantExpression : Expression {
  virtual ~ConstantExpression() = default;
  virtual llvm::SMRange getLocation() const = 0;
};
} // namespace chocopy::ast