#pragma once

#include "semantic/Type.h"

#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Value.h"

namespace chocopy::sema {
struct Variable {
  Variable(const Type& type) : type(type), allocation(nullptr) {};

  const Type& type;
  llvm::Value* allocation;
};
};