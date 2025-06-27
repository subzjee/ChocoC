#pragma once

#include "ast/ASTNode.h"
#include "ast/WithLocation.h"

namespace chocopy::ast {
struct Expression : ASTNode, WithLocation {
  virtual ~Expression() = default;
  virtual llvm::SMRange getLocation() const = 0;
};
} // namespace chocopy::ast