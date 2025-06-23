#pragma once

#include "SymbolTable.h"
#include "lexer/TokenType.h"
#include "parser/ParseContext.h"

#include "llvm/Support/ErrorHandling.h"

#include <variant>

namespace chocopy {
class TypeEnvironment {
public:
  TypeEnvironment(SymbolTable& symbol_table) : m_symbol_table(symbol_table) {};

  /// Get the type of a literal.
  /// @param ctx The literal context.
  /// @returns The type of a literal.
  [[nodiscard]] const Type& typeOf(const LiteralContext& ctx) {
    switch (ctx.getValue().getType()) {
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

  /// Get the type of a variable by name.
  /// @param name The name of the variable to get the type of.
  /// @returns The type of the variable \p name
  [[nodiscard]] const Type typeOf(const llvm::StringRef name) {
    const auto entry = m_symbol_table.getEntry(name);

    if (std::holds_alternative<Variable>(entry->get())) {
      return std::get<Variable>(entry->get()).type;
    }
  }

  /// Get the type of a variable.
  /// @param variable The variable to get the type of.
  /// @returns The type of \p variable.
  [[nodiscard]] const Type typeOf(const Variable& variable) {
    return variable.type;
  }

  /// Get the type of a constant expression.
  /// @param cexpr The constant expression to get the type of.
  /// @returns The type of \p cexpr.
  [[nodiscard]] const Type typeOf(const ConstantExprContext& cexpr) {
    return cexpr.visit([&](const auto& cexpr) { return typeOf(*cexpr); });
  }

  /// Get the type of a expression.
  /// @param cexpr The expression to get the type of.
  /// @returns The type of \p expr.
  [[nodiscard]] const Type typeOf(const ExprContext& expr) {
    return expr.visit([&](const auto& expr) { return typeOf(*expr); });
  }

  /// Check whether a type is a subclass of a second type.
  /// @param child The type to check.
  /// @param parent The parent class.
  /// @returns Whether \p first is a subclass of \p second
  [[nodiscard]] bool isSubclass(const Type& child, const Type& parent) const {
    if (child == *Type::getObjectType()) {
      return false;
    }

    return *child.super_class == parent;
  }

  /// Check whether a type conforms to a second type within the type
  /// environment.
  /// @param first The type to check for conformity.
  /// @param second The type to check against.
  /// @returns Whether \p first conforms to \p second
  [[nodiscard]] bool conformsTo(const Type& first, const Type& second) const {
    return (first == second) || isSubclass(first, second);
  }

  /// Check whether a type is assignment-compatible with a second type within
  /// the type environment.
  /// @param first The type to check for assignment compatibility.
  /// @param second The type to check against.
  /// @returns Whether \p first is assignment-compatible with to \p second
  [[nodiscard]] bool isAssignmentCompatible(const Type& first,
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

private:
  SymbolTable& m_symbol_table;
};
} // namespace chocopy