#pragma once

#include "ast/Expression.h"
#include "ast/Target.h"
#include "lexer/Token.h"

namespace chocopy::ast {
class Identifier : public Target, public Expression {
public:
  Identifier(const Token& identifier) : Target(TargetKind::TK_Identifier), Expression(ExpressionKind::EK_Identifier, /*is_cexpr*/ true), m_identifier(identifier) {
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

  /// Check whether \p expr is an Identifier.
  /// @returns Whether \p expr is an Identifier.
  static bool classof(const Expression* expr) {
    return expr->getKind() == ExpressionKind::EK_Identifier;
  }

  /// Check whether \p target is an Identifier.
  /// @returns Whether \p target is an Identifier.
  static bool classof(const Target* target) {
    return target->getKind() == TargetKind::TK_Identifier;
  }

private:
  const Token& m_identifier;
};
} // namespace chocopy::ast