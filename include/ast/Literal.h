#pragma once

#include "ast/ConstantExpression.h"
#include "lexer/Token.h"
#include <utility>
#include <variant>

namespace chocopy::ast {
class Literal : public ConstantExpression {
public:
  Literal(const Token& value) : m_value(value) {
    assert(value.isLiteral());

#ifndef NDEBUG
    switch (value.getType()) {
    case TokenType::STRING:
      [[fallthrough]];
    case TokenType::IDSTRING:
      assert(std::holds_alternative<std::string>(value.getValue()));
      break;
    case TokenType::TRUE:
      [[fallthrough]];
    case TokenType::FALSE:
      assert(std::holds_alternative<bool>(value.getValue()));
      break;
    case TokenType::INTLIT:
      assert(std::holds_alternative<std::int32_t>(value.getValue()));
      break;
    case TokenType::NONE:
      assert(std::holds_alternative<std::monostate>(value.getValue()));
      break;
    default:
      std::unreachable();
    }
#endif
  };

  /// Get the value of the literal.
  /// @returns The value.
  [[nodiscard]] const TokenValue& getValue() const {
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

  std::any accept(ASTVisitor& visitor) const override;

private:
  const Token& m_value;
};
} // namespace chocopy::ast