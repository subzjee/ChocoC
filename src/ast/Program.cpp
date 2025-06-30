#include "ast/Program.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
  std::any Program::accept(ASTVisitor& visitor) { return visitor.visit(*this); }
}