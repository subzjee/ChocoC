#pragma once

#include <any>

namespace chocopy {
struct ASTVisitor;

namespace ast {
struct ASTNode {
  virtual ~ASTNode() = default;

  virtual std::any accept(ASTVisitor& visitor) = 0;
};
}
}