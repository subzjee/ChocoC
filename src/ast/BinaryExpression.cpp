#include "ast/BinaryExpression.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
  std::any BinaryExpression::accept(ASTVisitor& visitor) const { return visitor.visit(*this); }
}