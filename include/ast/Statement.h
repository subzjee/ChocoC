#pragma once

#include "ast/ASTNode.h"
#include "ast/WithLocation.h"

namespace chocopy::ast {
struct Statement : ASTNode, WithLocation {
  virtual ~Statement() = default;
  virtual llvm::SMRange getLocation() const = 0;
};
} // namespace chocopy::ast