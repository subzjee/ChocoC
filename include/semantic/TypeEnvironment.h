#pragma once

#include "lexer/TokenType.h"
#include "parser/ParseContext.h"
#include "semantic/SymbolTable.h"

#include "llvm/Support/ErrorHandling.h"

#include <variant>

namespace chocopy {
class TypeEnvironment {
public:
  TypeEnvironment(const SymbolTable& symbol_table)
      : m_symbol_table(symbol_table) {};

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

  [[nodiscard]] const Type typeOf(const llvm::StringRef name) {
    const auto entry = m_symbol_table.getEntry(name);

    if (std::holds_alternative<Variable>(*entry)) {
      return std::get<Variable>(*entry).type;
    }
  }

  [[nodiscard]] const Type typeOf(const Variable& variable) {
    return variable.type;
  }

  /// Check whether a type is a subclass of a second type.
  /// @param first The type to check.
  /// @param second The parent class.
  /// @returns Whether \p first is a subclass of \p second
  [[nodiscard]] bool isSubclass(const Type& first, const Type& second) const {
    return *first.super_class == second;
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
  const SymbolTable& m_symbol_table;
};
} // namespace chocopy