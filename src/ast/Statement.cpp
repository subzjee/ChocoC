#include "ast/Statement.h"
#include "ast/ASTNode.h"
#include "ast/Expression.h"

namespace chocopy::ast {
bool Statement::classof(const ASTNode* node) {
  return llvm::isa<Expression>(node) || node->getKind() == NK_AssignmentStatement;
}
};