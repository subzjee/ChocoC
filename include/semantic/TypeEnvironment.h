#pragma once

#include "SymbolTable.h"

#include "ast/Expression.h"
#include "ast/ConstantExpression.h"

#include "llvm/Support/ErrorHandling.h"

namespace chocopy {
class TypeEnvironment {
public:
  TypeEnvironment(SymbolTable& symbol_table) : m_symbol_table(symbol_table) {};

  /// Get the type of a literal.
  /// @param ctx The literal context.
  /// @returns The type of a literal.
  [[nodiscard]] const Type& typeOf(const ast::Literal& ctx);

  /// Get the type of a variable by name.
  /// @param name The name of the variable to get the type of.
  /// @returns The type of the variable \p name
  [[nodiscard]] const Type typeOf(const llvm::StringRef name);

  /// Get the type of a variable.
  /// @param variable The variable to get the type of.
  /// @returns The type of \p variable.
  [[nodiscard]] const Type typeOf(const Variable& variable);

  /// Get the type of a constant expression.
  /// @param cexpr The constant expression to get the type of.
  /// @returns The type of \p cexpr.
  [[nodiscard]] const Type typeOf(const ast::ConstantExpression& cexpr);

  /// Get the type of a expression.
  /// @param cexpr The expression to get the type of.
  /// @returns The type of \p expr.
  [[nodiscard]] const Type typeOf(const ast::Expression& expr);

  /// Check whether a type is a subclass of a second type.
  /// @param child The type to check.
  /// @param parent The parent class.
  /// @returns Whether \p first is a subclass of \p second
  [[nodiscard]] bool isSubclass(const Type& child, const Type& parent) const;

  /// Check whether a type conforms to a second type within the type
  /// environment.
  /// @param first The type to check for conformity.
  /// @param second The type to check against.
  /// @returns Whether \p first conforms to \p second
  [[nodiscard]] bool conformsTo(const Type& first, const Type& second) const;

  /// Check whether a type is assignment-compatible with a second type within
  /// the type environment.
  /// @param first The type to check for assignment compatibility.
  /// @param second The type to check against.
  /// @returns Whether \p first is assignment-compatible with to \p second
  [[nodiscard]] bool isAssignmentCompatible(const Type& first,
                                            const Type& second) const;

private:
  SymbolTable& m_symbol_table;
};
} // namespace chocopy