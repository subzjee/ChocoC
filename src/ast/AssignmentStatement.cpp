#include "ast/AssignmentStatement.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
  std::any AssignmentStatement::accept(ASTVisitor& visitor) { return visitor.visit(*this); }
}