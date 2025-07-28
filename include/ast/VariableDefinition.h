#pragma once

#include "ast/ASTNode.h"
#include "ast/Literal.h"
#include "ast/TypedVariable.h"
#include "lexer/Token.h"

#include <memory>

namespace chocopy::ast {
class VariableDefinition : public ASTNode {
public:
  VariableDefinition(std::unique_ptr<TypedVariable> typed_var,
                     std::unique_ptr<Literal> value)
      : m_name(std::move(typed_var->getName())),
        m_type(std::move(typed_var->getType())), m_value(std::move(value)) {};

  /// Get the name of the variable.
  /// @returns The name.
  [[nodiscard]] const std::unique_ptr<Identifier>& getName() const {
    return m_name;
  };

  /// Get the type of the variable.
  /// @returns The type.
  [[nodiscard]] const std::unique_ptr<Type>& getType() const { return m_type; };

  /// Get the value of the variable.
  /// @returns The value.
  [[nodiscard]] const std::unique_ptr<Literal>& getValue() const {
    return m_value;
  };

  std::any accept(ASTVisitor& visitor) const override;

private:
  std::unique_ptr<Identifier> m_name;
  std::unique_ptr<Type> m_type;
  std::unique_ptr<Literal> m_value;
};
} // namespace chocopy::ast