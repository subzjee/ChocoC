#include "ast/Target.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
  std::any Target::accept(ASTVisitor& visitor) { return visitor.visit(*this); }
}