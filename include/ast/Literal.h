#pragma once

#include "ast/ConstantExpression.h"
#include "lexer/Token.h"
#include <utility>

namespace chocopy::ast {
class Literal : public ConstantExpression {
public:
  Literal(const Token &value) : m_value(value) { assert(value.isLiteral()); };

  /// Get the value of the literal.
  /// @returns The value.
  [[nodiscard]] const TokenValue &getValue() const {
    return m_value.getValue();
  };

  /// Get the type of the literal.
  /// @return The type.
  [[nodiscard]] const llvm::StringRef getType() const {
    switch (m_value.getType()) {
    case TokenType::TRUE:
      [[fallthrough]];
    case TokenType::FALSE:
      return "bool";
    case TokenType::IDSTRING:
      [[fallthrough]];
    case TokenType::STRING:
      return "str";
    case TokenType::INTLIT:
      return "int";
    case TokenType::NONE:
      return "<None>";
    default:
      std::unreachable();
    }
  };

  /// Get the location of the literal.
  /// @returns The location.
  [[nodiscard]] llvm::SMRange getLocation() const override {
    return m_value.getLocation();
  };

  std::any accept(ASTVisitor &visitor) const override;

private:
  const Token &m_value;
};
} // namespace chocopy::ast