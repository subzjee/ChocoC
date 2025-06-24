#pragma once

#include "llvm/Support/SMLoc.h"

namespace chocopy {
/// Abstract base class for nodes that have a location.
struct WithLocation {
  virtual ~WithLocation() = default;

  [[nodiscard]] virtual llvm::SMRange getLocation() const = 0;
};
} // namespace chocopy