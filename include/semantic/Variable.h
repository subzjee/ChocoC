#pragma once

#include "llvm/IR/Value.h"

#include "semantic/Type.h"

namespace chocopy::sema {
struct Variable {
  explicit Variable(const Type& type) : type(type), allocation(nullptr) {};

  const Type& type;
  llvm::Value* allocation;
};
}; // namespace chocopy::sema
