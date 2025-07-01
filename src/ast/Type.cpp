#include "ast/Type.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
  std::any Type::accept(ASTVisitor& visitor) const { return visitor.visit(*this); }
}