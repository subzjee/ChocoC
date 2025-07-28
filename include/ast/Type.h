#pragma once

#include "ast/ASTNode.h"
#include "ast/WithLocation.h"
#include "lexer/Token.h"

namespace chocopy::ast {
class Type : public ASTNode, public WithLocation {
public:
  Type(const Token& base_type, unsigned int dimension = 0)
      : m_base_type(base_type), m_dimension(dimension) {};

  /// Get the base type.
  /// @returns The base type.
  [[nodiscard]] const Token& getBaseType() const { return m_base_type; };

  /// Get the dimension of the type.
  /// @returns The dimension.
  [[nodiscard]] unsigned int getDimension() const { return m_dimension; };

  /// Get the string representation of the type.
  /// @returns The string representation.
  [[nodiscard]] std::string getText() const {
    return std::string(m_dimension, '[') + m_base_type.getText().str() +
           std::string(m_dimension, ']');
  };

  [[nodiscard]] llvm::SMRange getLocation() const override {
    return m_base_type.getLocation();
  };

  std::any accept(ASTVisitor& visitor) const override;

private:
  const Token& m_base_type;
  const unsigned int m_dimension;
};
} // namespace chocopy::ast