#include "ast/Literal.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
  std::any Literal::accept(ASTVisitor& visitor) { return visitor.visit(*this); }
}