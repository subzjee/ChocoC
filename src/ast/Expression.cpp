#include "ast/Expression.h"
#include "ast/ASTNode.h"

namespace chocopy::ast {
bool Expression::classof(const ASTNode* node) {
  return node->getKind() > NK_Expression && node->getKind() < NK_SentinelExpression;
}
}