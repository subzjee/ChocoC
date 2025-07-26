#pragma once

#include "lexer/Token.h"

#include <functional>
#include <optional>
#include <span>

namespace chocopy {
class TokenStream {
public:
  TokenStream(std::span<const Token> tokens) : m_tokens(tokens) {};

  [[nodiscard]] std::optional<std::reference_wrapper<const Token>>
  peek(const int n = 0) const;

  std::optional<std::reference_wrapper<const Token>> advance();

private:
  std::span<const Token> m_tokens;
  std::size_t m_current_idx{0};
};
} // namespace chocopy