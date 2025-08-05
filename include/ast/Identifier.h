#pragma once

#include "ast/ASTNode.h"
#include "ast/Expression.h"
#include "lexer/Token.h"

namespace chocopy::ast {
class Identifier : public Expression {
public:
  Identifier(const Token& identifier) : Expression(NK_Identifier, /*is_cexpr*/ true), m_identifier(identifier) {
    assert(identifier.getType() == TokenType::ID);
  };

  /// Get the value.
  /// @returns The value.
  [[nodiscard]] llvm::StringRef getValue() const {
    return m_identifier.getText();
  };

  /// Get the source location.
  /// @returns The location.
  [[nodiscard]] llvm::SMRange getLocation() const override {
    return m_identifier.getLocation();
  };

  std::any accept(ASTVisitor& visitor) const override;

  /// Check whether \p node is an Identifier.
  /// @returns Whether \p node is an Identifier.
  static bool classof(const ASTNode* node);

private:
  const Token& m_identifier;
};
} // namespace chocopy::ast