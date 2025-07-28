#pragma once

#include "semantic/Type.h"

#include "llvm/IR/Value.h"

namespace chocopy::sema {
struct Variable {
  explicit Variable(const Type& type) : type(type), allocation(nullptr) {};

  const Type& type;
  llvm::Value* allocation;
};
}; // namespace chocopy::sema