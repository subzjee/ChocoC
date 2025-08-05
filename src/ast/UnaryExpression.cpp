#include "ast/UnaryExpression.h"
#include "ast/ASTNode.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
bool UnaryExpression::classof(const ASTNode* node) {
  return node->getKind() == NK_UnaryExpression;
}

std::any UnaryExpression::accept(ASTVisitor& visitor) const {
  return visitor.visit(*this);
}
} // namespace chocopy::ast