#include "ast/Literal.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
  std::any Literal::accept(ASTVisitor& visitor) const { return visitor.visit(*this); }
}