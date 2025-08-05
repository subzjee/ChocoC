#include "ast/GroupingExpression.h"
#include "ast/ASTNode.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
bool GroupingExpression::classof(const ASTNode *node) {
  return node->getKind() == NK_GroupingExpression;
}

std::any GroupingExpression::accept(ASTVisitor& visitor) const {
  return visitor.visit(*this);
}
} // namespace chocopy::ast
