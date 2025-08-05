#include "ast/VariableDefinition.h"
#include "ast/ASTNode.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
bool VariableDefinition::classof(const ASTNode* node) {
  return node->getKind() == NK_VariableDefinition;
}

std::any VariableDefinition::accept(ASTVisitor& visitor) const {
  return visitor.visit(*this);
}
} // namespace chocopy::ast