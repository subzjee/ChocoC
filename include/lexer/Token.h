#pragma once

#include "lexer/TokenType.h"

#include "llvm/Support/SMLoc.h"

#include <variant>

namespace chocopy {
using TokenValue =
    std::variant<std::monostate, bool, std::int32_t, std::string>;

class Token {
public:
  /// Constructor for a Token.
  /// @param type The token type of the token.
  /// @param text The value of the token.
  /// @param location The location of the token in the source file.
  Token(const TokenType type, const TokenValue value,
        const llvm::SMRange location)
      : m_value{value}, m_location{location}, m_type{type} {};

  constexpr bool operator==(const Token& other) const {
    return m_value == other.getValue() && m_type == other.getType() &&
           m_location.Start == other.getLocation().Start &&
           m_location.End == other.getLocation().End;
  }

  [[nodiscard]] const TokenValue& getValue() const { return m_value; };
  [[nodiscard]] const llvm::SMRange& getLocation() const { return m_location; };
  [[nodiscard]] const TokenType& getType() const { return m_type; };

  [[nodiscard]] bool isLiteral() const {
    return m_type == TokenType::NONE || m_type == TokenType::FALSE ||
           m_type == TokenType::TRUE || m_type == TokenType::INTLIT ||
           m_type == TokenType::IDSTRING || m_type == TokenType::STRING;
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