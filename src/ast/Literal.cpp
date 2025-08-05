#include "ast/Literal.h"
#include "ast/ASTNode.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
bool Literal::classof(const ASTNode* node) {
  return node->getKind() == NK_Literal;
}

std::any Literal::accept(ASTVisitor& visitor) const {
  return visitor.visit(*this);
}
} // namespace chocopy::ast