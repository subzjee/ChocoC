#include "ast/Program.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
std::any Program::accept(ASTVisitor& visitor) const {
  return visitor.visit(*this);
}
} // namespace chocopy::ast