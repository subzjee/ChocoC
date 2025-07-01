#include "semantic/TypeEnvironment.h"
#include "ast/BinaryExpression.h"
#include "ast/ConstantExpression.h"
#include "ast/Expression.h"
#include "ast/Literal.h"

namespace chocopy {
[[nodiscard]] const Type& TypeEnvironment::typeOf(const ast::Literal& ctx) {
  const auto type = ctx.getType();

  if (type == "bool") {
    return *Type::getBooleanType();
  } else if (type == "str") {
    return *Type::getStringType();
  } else if (type == "int") {
    return *Type::getIntegerType();
  } else {
    return *Type::getNoneType();
  }
}

[[nodiscard]] const Type& TypeEnvironment::typeOf(const llvm::StringRef name) {
  const auto entry = m_symbol_table.getEntry(name);

  if (std::holds_alternative<Variable>(entry->get())) {
    return std::get<Variable>(entry->get()).type;
  }
}

[[nodiscard]] inline const Type&
TypeEnvironment::typeOf(const Variable& variable) {
  return variable.type;
}

[[nodiscard]] const Type&
TypeEnvironment::typeOf(const ast::ConstantExpression& cexpr) {
  if (auto literal = dynamic_cast<const ast::Literal*>(&cexpr)) {
    return typeOf(*literal);
  } else if (auto bin = dynamic_cast<
                 const ast::BinaryExpression<ast::ConstantExpression>*>(
                 &cexpr)) {
    return typeOf(*bin);
  }
}

[[nodiscard]] const Type& TypeEnvironment::typeOf(const ast::Expression& expr) {
  if (auto constant_expression =
          dynamic_cast<const ast::ConstantExpression*>(&expr)) {
    return typeOf(*constant_expression);
  }
}

[[nodiscard]] const Type&
TypeEnvironment::typeOf(const ast::BinaryExpression<ast::Expression>& expr) {
  return *Type::getBooleanType();
}

[[nodiscard]] const Type& TypeEnvironment::typeOf(
    const ast::BinaryExpression<ast::ConstantExpression>& expr) {
  return *Type::getBooleanType();
}

[[nodiscard]] bool TypeEnvironment::isSubclass(const Type& child,
                                               const Type& parent) const {
  if (child == *Type::getObjectType()) {
    return false;
  }

  return *child.super_class == parent;
}

[[nodiscard]] bool TypeEnvironment::conformsTo(const Type& first,
                                               const Type& second) const {
  return (first == second) || isSubclass(first, second);
}

[[nodiscard]] bool
TypeEnvironment::isAssignmentCompatible(const Type& first,
                                        const Type& second) const {
  // 1. Ordinary subtyping.
  if (conformsTo(first, second)) {
    return true;
  }

  // 2. Can not assign None to a literal.
  if ((first.isNone() && !second.isInteger() && !second.isBoolean() &&
       !second.isString())) {
    return true;
  }

  // 3. Can assign empty list to any list type.
  if ((first.isEmpty() && second.isList())) {
    return true;
  }

  // 4. Allow assigning [None] to variables of type [T] where T is
  // assignment-compatible with <None>.
  if ((first.dimension == 1 && first.isNone() && second.dimension == 1 &&
       isAssignmentCompatible(*Type::getNoneType(), second))) {
    return true;
  }

  return false;
}
} // namespace chocopy