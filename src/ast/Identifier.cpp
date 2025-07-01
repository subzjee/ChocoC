#include "ast/Identifier.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
std::any Identifier::accept(ASTVisitor& visitor) const {
  return visitor.visit(*this);
}
} // namespace chocopy::ast