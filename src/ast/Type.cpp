#include "ast/Type.h"
#include "ast/ASTNode.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
bool Type::classof(const ASTNode* node) {
  return node->getKind() == NK_Type;
}

std::any Type::accept(ASTVisitor& visitor) const {
  return visitor.visit(*this);
}
} // namespace chocopy::ast