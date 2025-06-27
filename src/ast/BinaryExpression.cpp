#include "ast/BinaryExpression.h"
#include "ASTVisitor.h"

namespace chocopy::ast {
  std::any BinaryExpression::accept(ASTVisitor& visitor) { return visitor.visit(*this); }
}