#pragma once

#include "ast/ConstantExpression.h"
#include "ast/Target.h"
#include "lexer/Token.h"

namespace chocopy::ast {
class Identifier : public ConstantExpression {
public:
  Identifier(const Token& identifier) : m_identifier(identifier) {
    assert(identifier.getType() == TokenType::ID);
  };

  /// Get the name of the identifier.
  /// @returns The value.
  [[nodiscard]] const llvm::StringRef getValue() const {
    return std::get<std::string>(m_identifier.getValue());
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