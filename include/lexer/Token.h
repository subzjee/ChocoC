#pragma once

#include "lexer/TokenType.h"

#include "llvm/Support/SMLoc.h"

#include <variant>

#include <iostream>

namespace chocopy {
using TokenValue =
    std::variant<std::monostate, bool, std::int32_t, std::string>;

class Token {
public:
  /// Constructor for a Token.
  /// @param type The token type of the token.
  /// @param text The value of the token.
  /// @param location The location of the token in the source file.
  Token(const TokenType type, const TokenValue value = std::monostate{},
        const llvm::SMRange location = {})
      : m_value{value}, m_location{location}, m_type{type} {
#ifndef NDEBUG
    switch (type) {
    case TokenType::TRUE:
      [[fallthrough]];
    case TokenType::FALSE:
      assert(std::holds_alternative<bool>(value));
      break;
    case TokenType::STRING:
      [[fallthrough]];
    case TokenType::IDSTRING:
      [[fallthrough]];
    case TokenType::ID:
      assert(std::holds_alternative<std::string>(value));
      break;
    case TokenType::INTLIT:
      assert(std::holds_alternative<std::int32_t>(value));
      break;
    default:
      assert(std::holds_alternative<std::monostate>(value));
    }
#endif
  };

  constexpr bool operator==(const Token& other) const {
    return m_value == other.getValue() && m_type == other.getType();
  }

  [[nodiscard]] const TokenValue& getValue() const { return m_value; };
  [[nodiscard]] const llvm::SMRange& getLocation() const { return m_location; };
  [[nodiscard]] const TokenType& getType() const { return m_type; };

  [[nodiscard]] bool isLiteral() const {
    return m_type == TokenType::NONE || m_type == TokenType::FALSE ||
           m_type == TokenType::TRUE || m_type == TokenType::INTLIT ||
           m_type == TokenType::IDSTRING || m_type == TokenType::STRING;
  }

  [[nodiscard]] bool isBinOp() const {
    return m_type == TokenType::PLUS || m_type == TokenType::MINUS ||
           m_type == TokenType::MULT || m_type == TokenType::DIV ||
           m_type == TokenType::MOD || m_type == TokenType::EQUAL ||
           m_type == TokenType::NEQUAL || m_type == TokenType::LESSEQ ||
           m_type == TokenType::GREATEQ || m_type == TokenType::LESS ||
           m_type == TokenType::GREAT || m_type == TokenType::IS;
  }

private:
  const TokenValue m_value;
  const llvm::SMRange m_location;
  const TokenType m_type;
};

/// Compare the token type of an optional token to a given type.
/// @param opt_token The optional token.
/// @param type The type to compare to.
/// @returns Whether the token type of \p opt_token is the same as \p type.
constexpr bool operator==(const std::optional<chocopy::Token> opt_token,
                          chocopy::TokenType type) {
  return opt_token.has_value() && opt_token->getType() == type;
};

/// Compare the token type of an optional token to a given type.
/// @param type The type to compare to.
/// @param opt_token The optional token.
/// @returns Whether the token type of \p opt_token is the same as \p type.
constexpr bool operator==(chocopy::TokenType type,
                          const std::optional<chocopy::Token> opt_token) {
  return opt_token == type;
};
} // namespace chocopy