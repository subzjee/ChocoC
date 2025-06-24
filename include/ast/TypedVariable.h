#pragma once

#include "ast/Type.h"
#include "lexer/Token.h"

#include <memory>

namespace chocopy::ast {
class TypedVariable {
public:
  TypedVariable(const Token& name, std::unique_ptr<Type>& type)
      : m_name(name), m_type(std::move(type)) {};

  /// Get the name.
  /// @returns The name.
  [[nodiscard]] const Token& getName() const { return m_name; };

  /// Get the type.
  /// @returns The type.
  [[nodiscard]] std::unique_ptr<Type>& getType() { return m_type; };

private:
  const Token& m_name;
  std::unique_ptr<Type> m_type;
};
} // namespace chocopy::ast