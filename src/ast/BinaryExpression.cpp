#include "ast/BinaryExpression.h"
#include "ast/ASTNode.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
bool BinaryExpression::classof(const ASTNode *node) {
  return node->getKind() == NK_BinaryExpression;
}

std::any BinaryExpression::accept(ASTVisitor& visitor) const {
  return visitor.visit(*this);
}
} // namespace chocopy::ast