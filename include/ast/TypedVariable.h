#pragma once

#include "ast/Identifier.h"
#include "ast/Type.h"
#include "lexer/Token.h"

#include <memory>

namespace chocopy::ast {
class TypedVariable {
public:
  TypedVariable(std::unique_ptr<Identifier> name, std::unique_ptr<Type> type)
      : m_name(std::move(name)), m_type(std::move(type)) {};

  /// Get the name.
  /// @returns The name.
  [[nodiscard]] std::unique_ptr<Identifier>& getName() { return m_name; };

  /// Get the type.
  /// @returns The type.
  [[nodiscard]] std::unique_ptr<Type>& getType() { return m_type; };

private:
  std::unique_ptr<Identifier> m_name;
  std::unique_ptr<Type> m_type;
};
} // namespace chocopy::ast