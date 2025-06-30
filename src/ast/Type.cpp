#include "ast/Type.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
  std::any Type::accept(ASTVisitor& visitor) { return visitor.visit(*this); }
}