#pragma once

#include "lexer/Token.h"
#include "utility/OptionalRef.h"

#include <span>

namespace chocopy {
class TokenStream {
public:
  TokenStream(std::span<const Token> tokens) : m_tokens(tokens) {};

  [[nodiscard]] OptionalRef<const Token>
  peek(const int n = 0) const;

  OptionalRef<const Token> advance();

private:
  std::span<const Token> m_tokens;
  std::size_t m_current_idx{0};
};
} // namespace chocopy