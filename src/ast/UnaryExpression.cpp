#include "ast/UnaryExpression.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
std::any UnaryExpression::accept(ASTVisitor& visitor) const {
  return visitor.visit(*this);
}
} // namespace chocopy::ast