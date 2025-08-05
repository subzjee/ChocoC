#include "ast/AssignmentStatement.h"
#include "ast/ASTNode.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
bool AssignmentStatement::classof(const ASTNode* node) {
  return node->getKind() == NK_AssignmentStatement;
}

std::any AssignmentStatement::accept(ASTVisitor& visitor) const {
  return visitor.visit(*this);
}
} // namespace chocopy::ast