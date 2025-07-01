#include "ast/BinaryExpression.h"
#include "ast/ASTVisitor.h"
#include "ast/ConstantExpression.h"
#include "ast/Expression.h"

namespace chocopy::ast {
template <>
std::any
BinaryExpression<ast::ConstantExpression>::accept(ASTVisitor& visitor) const {
  return visitor.visit(*this);
}
template <>
std::any BinaryExpression<ast::Expression>::accept(ASTVisitor& visitor) const {
  return visitor.visit(*this);
}
} // namespace chocopy::ast