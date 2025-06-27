#include "ast/Literal.h"
#include "ASTVisitor.h"

namespace chocopy::ast {
  std::any Literal::accept(ASTVisitor& visitor) { return visitor.visit(*this); }
}