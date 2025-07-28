#pragma once

#include "llvm/IR/Value.h"

namespace chocopy::ir {
struct Variable {
  Variable() = default;

  llvm::Value* allocation{nullptr};
};
}; // namespace chocopy::ir