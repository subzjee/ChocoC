#pragma once

#include "lexer/TokenType.h"

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/SMLoc.h"

namespace chocopy {
struct Token {
  /// Constructor for a Token.
  /// @param type The token type of the token.
  /// @param text The value of the token.
  /// @param location The location of the token in the source file.
  Token(const TokenType type, const llvm::StringRef text,
        const llvm::SMRange location)
      : text{text}, location{location}, type{type} {};

  const llvm::StringRef text;
  const llvm::SMRange location;
  const TokenType type;
};
} // namespace chocopy