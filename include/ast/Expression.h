#pragma once

#include "ast/WithLocation.h"

namespace chocopy::ast {

struct Expression : WithLocation {
  virtual ~Expression() = default;
  virtual llvm::SMRange getLocation() const = 0;

  template <typename Visitor> auto visit(Visitor&& visitor) const {
    return visitor(*this);
  }
};
} // namespace chocopy::ast