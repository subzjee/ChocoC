#pragma once

#include "lexer/Token.h"
#include "utility/OptionalRef.h"

#include <span>

namespace chocopy {
class TokenStream {
public:
  TokenStream(std::span<const Token> tokens) : m_tokens(tokens) {};

  /// Get the token \p n positions ahead of the current token.
  /// @returns The token \p n positions ahead.
  [[nodiscard]] OptionalRef<const Token> peek(const int n = 0) const;

  /// Advance the stream by one position.
  /// @returns The current token after advancing.
  OptionalRef<const Token> advance();

private:
  std::span<const Token> m_tokens;
  std::size_t m_current_idx{0};
};
} // namespace chocopy