#include "ast/Identifier.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
  std::any Identifier::accept(ASTVisitor& visitor) { return visitor.visit(*this); }
}