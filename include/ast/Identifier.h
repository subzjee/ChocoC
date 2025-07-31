#pragma once

#include "ast/Expression.h"
#include "ast/Target.h"
#include "lexer/Token.h"

namespace chocopy::ast {
class Identifier : public Target, public Expression {
public:
  Identifier(const Token& identifier) : Expression(/*is_constant_expression*/ true), m_identifier(identifier) {
    assert(identifier.getType() == TokenType::ID);
  };

  /// Get the name of the identifier.
  /// @returns The value.
  [[nodiscard]] llvm::StringRef getValue() const {
    return m_identifier.getText();
  };

  /// Get the location of the identifier.
  /// @returns The location.
  [[nodiscard]] llvm::SMRange getLocation() const override {
    return m_identifier.getLocation();
  };

  std::any accept(ASTVisitor& visitor) const override;

private:
  const Token& m_identifier;
};
} // namespace chocopy::ast