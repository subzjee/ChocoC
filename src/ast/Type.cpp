#include "ast/Type.h"
#include "ASTVisitor.h"

namespace chocopy::ast {
  std::any Type::accept(ASTVisitor& visitor) { return visitor.visit(*this); }
}