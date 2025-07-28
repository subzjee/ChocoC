#pragma once

#include "SymbolTable.h"

#include "ast/Identifier.h"

namespace chocopy {
class TypeEnvironment {
public:
  TypeEnvironment(SymbolTable& symbol_table) : m_symbol_table(symbol_table) {};

  /// Get the type of a variable.
  /// @param variable The variable to get the type of.
  /// @returns The type of \p variable.
  [[nodiscard]] const Type& typeOf(const ast::Identifier& variable);

private:
  SymbolTable& m_symbol_table;
};
} // namespace chocopy