#include "ast/Statement.h"
#include "ast/Expression.h"

namespace chocopy::ast {
bool Statement::classof(const ASTNode* node) {
  return llvm::isa<Expression>(node);
}
};