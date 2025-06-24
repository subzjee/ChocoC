#include "semantic/TypeEnvironment.h"

namespace chocopy {
[[nodiscard]] const Type& TypeEnvironment::typeOf(const LiteralContext& ctx) {
  switch (ctx.getType()) {
  case TokenType::INTLIT:
    return *Type::getIntegerType();
  case TokenType::STRING:
    [[fallthrough]];
  case TokenType::IDSTRING:
    return *Type::getStringType();
  case TokenType::FALSE:
    [[fallthrough]];
  case TokenType::TRUE:
    return *Type::getBooleanType();
  case TokenType::NONE:
    return *Type::getNoneType();
  }
}

[[nodiscard]] const Type TypeEnvironment::typeOf(const llvm::StringRef name) {
  const auto entry = m_symbol_table.getEntry(name);

  if (std::holds_alternative<Variable>(entry->get())) {
    return std::get<Variable>(entry->get()).type;
  }
}

[[nodiscard]] inline const Type TypeEnvironment::typeOf(const Variable& variable) {
  return variable.type;
}

[[nodiscard]] const Type TypeEnvironment::typeOf(const ConstantExprContext& cexpr) {
  return cexpr.visit([&](const auto& cexpr) { return typeOf(*cexpr); });
}

[[nodiscard]] const Type TypeEnvironment::typeOf(const ExprContext& expr) {
  return expr.visit([&](const auto& expr) { return typeOf(*expr); });
}

[[nodiscard]] bool TypeEnvironment::isSubclass(const Type& child, const Type& parent) const {
  if (child == *Type::getObjectType()) {
    return false;
  }

  return *child.super_class == parent;
}

[[nodiscard]] bool TypeEnvironment::conformsTo(const Type& first, const Type& second) const {
  return (first == second) || isSubclass(first, second);
}

[[nodiscard]] bool TypeEnvironment::isAssignmentCompatible(const Type& first,
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