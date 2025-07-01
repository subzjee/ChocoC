#include "ast/AssignmentStatement.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
std::any AssignmentStatement::accept(ASTVisitor& visitor) const {
  return visitor.visit(*this);
}
} // namespace chocopy::ast