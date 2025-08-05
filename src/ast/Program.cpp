#include "ast/Program.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
bool Program::classof(const ASTNode* node) {
  return node->getKind() == NK_Program;
}

std::any Program::accept(ASTVisitor& visitor) const {
  return visitor.visit(*this);
}
} // namespace chocopy::ast