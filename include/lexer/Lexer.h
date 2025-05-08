#pragma once

#include "lexer/Token.h"

#include "llvm/Support/SourceMgr.h"

#include <span>
#include <stack>
#include <vector>

namespace chocopy {
class Lexer {
public:
  /// Constructor for a Lexer.
  /// @param buffer_id The ID of the buffer within \p SourceManager.
  /// @param source_manager The source manager managing the buffer.
  Lexer(const unsigned buffer_id, llvm::SourceMgr &source_manager);

  Lexer(const Lexer &) = delete;
  Lexer &operator=(const Lexer &) = delete;

  /// Tokenize the source code into its constituent tokens.
  /// @returns The tokens.
  [[nodiscard]] std::span<const Token> lex();

  [[nodiscard]] std::span<const llvm::SMDiagnostic> getDiagnostics() const;

private:
  /// Get the current character.
  /// @returns The current character.
  [[nodiscard]] std::optional<char> peek() const;

  /// Get the current character and advance by one position.
  /// @returns The current character.
  std::optional<char> advance();

  /// Check if the current character matches any of the given characters.
  /// @param chars The characters to compare the current character to.
  /// @returns Whether the current character matches any of the characters in \p
  /// chs
  template <typename... Chars> [[nodiscard]] bool match(const Chars &...chs) {
    const auto current_char = peek();

    if (!current_char) {
      return false;
    }

    if (((current_char == chs) || ...)) {
      return true;
    }

    return false;
  }

  /// Check if the current character matches the given character.
  /// Unlike `match`, it will emit an error if no match is found.
  /// This is should be used for multi-character tokens where a single character
  /// token is not valid. An example of this being the division operator //,
  /// where / is not valid. Whereas the right-arrow operator -> should still use
  /// `match` because - is a valid operator.
  /// @param ch The characters to compare the current character to.
  /// @returns Whether the current character matches any of the characters in \p
  /// ch
  [[nodiscard]] bool expect(const char ch);

  /// Add a token to the found tokens.
  /// It will automatically obtain the value and location.
  /// @param type The type of the token.
  void addToken(TokenType type);

  /// Skip any characters within the comment.
  void skipComment();

  /// Perform scanning for whitespace.
  void scanWhitespace();

  /// Perform scanning for a number.
  void scanNumber();

  /// Perform scanning for an identifier or keyword.
  void scanIdOrKeyword();

  /// Perform scanning for a string literal.
  void scanString();

  /// Handle a new line character.
  void handleNewLine();

  /// Handle indentation.
  void handleIndentation();

  /// Get the text of the current lexeme.
  /// @returns The text of the current lexeme.
  llvm::StringRef getCurrentLexeme() const;

  /// Get the start location of the current lexeme.
  /// @returns The start location of the current lexeme.
  llvm::SMLoc getCurrentLexemeStartLocation() const;

  /// Get the end location of the current lexeme.
  /// @returns The end location of the current lexeme.
  llvm::SMLoc getCurrentLexemeEndLocation() const;

  /// Get the location range of the current lexeme.
  /// @returns The location range of the current lexeme.
  llvm::SMRange getCurrentLexemeLocation() const;

  /// Add an error diagnostic.
  /// @param message The message to print.
  /// @param location The location where the error occurred.
  /// @param fixits Suggestions on how to fix the error within the line.
  void addError(const llvm::Twine &message, llvm::SMRange location,
                llvm::ArrayRef<llvm::SMFixIt> fixits = {});

  std::stack<std::size_t, llvm::SmallVector<std::size_t>> m_indentation_levels{
      {0}};

  std::vector<Token> m_tokens;
  std::vector<llvm::SMDiagnostic> m_diagnostics;
  llvm::SourceMgr &m_source_manager;

  const char *m_lexeme_start; // Pointer to the start of the current lexeme.
  const char *const m_buffer_end;  // Pointer to the end of the buffer.
  std::size_t m_lexeme_offset{0}; // Length of the current lexeme.

  bool m_is_blank_line{true};  // Defaults to true because an empty program is a blank line.
};
} // namespace chocopy