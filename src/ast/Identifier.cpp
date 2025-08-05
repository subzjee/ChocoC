#include "ast/Identifier.h"
#include "ast/ASTNode.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
bool Identifier::classof(const ASTNode* node) {
  return node->getKind() == NK_Identifier;
}

std::any Identifier::accept(ASTVisitor& visitor) const {
  return visitor.visit(*this);
}
} // namespace chocopy::ast