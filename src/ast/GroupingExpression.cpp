#include "ast/GroupingExpression.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
std::any GroupingExpression::accept(ASTVisitor& visitor) const {
  return visitor.visit(*this);
}
} // namespace chocopy::ast
