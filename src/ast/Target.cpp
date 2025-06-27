#include "ast/Target.h"
#include "ASTVisitor.h"

namespace chocopy::ast {
  std::any Target::accept(ASTVisitor& visitor) { return visitor.visit(*this); }
}