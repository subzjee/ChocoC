#pragma once

#include "ast/ASTNode.h"
#include "ast/WithLocation.h"

namespace chocopy::ast {
struct Target : ASTNode, WithLocation {
  virtual ~Target() = default;
  virtual llvm::SMRange getLocation() const = 0;
};
} // namespace chocopy::ast