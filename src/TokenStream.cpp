#include "TokenStream.h"

namespace chocopy {
OptionalRef<const Token>
TokenStream::peek(const int n) const {
  const auto idx = m_current_idx + n;

  if (idx < 0 || idx >= m_tokens.size()) {
    return std::nullopt;
  }

  return std::cref(m_tokens[idx]);
}

OptionalRef<const Token> TokenStream::advance() {
  const auto current_token = peek();

  if (current_token) {
    ++m_current_idx;
  }

  return current_token;
}
} // namespace chocopy