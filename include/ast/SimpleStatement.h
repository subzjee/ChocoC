#pragma once

#include "ast/Statement.h"

namespace chocopy::ast {
struct SimpleStatement : Statement {
  virtual ~SimpleStatement() = default;
  virtual llvm::SMRange getLocation() const = 0;
};
} // namespace chocopy::ast