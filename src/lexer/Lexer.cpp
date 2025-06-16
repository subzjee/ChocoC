#include "lexer/Lexer.h"
#include "DiagnosticsManager.h"
#include "lexer/TokenType.h"

#include "llvm/ADT/StringExtras.h"
#include "llvm/Support/FormatVariadic.h"
#include "llvm/Support/SMLoc.h"
#include "llvm/Support/SourceMgr.h"

using namespace llvm;

namespace chocopy {
Lexer::Lexer(const unsigned buffer_id, const SourceMgr& source_manager, DiagnosticsManager& diagnostics_manager)
    : m_source_manager(source_manager),
      m_diag_manager(diagnostics_manager),
      m_lexeme_start(source_manager.getMemoryBuffer(buffer_id)->getBufferStart()),
      m_buffer_end(source_manager.getMemoryBuffer(buffer_id)->getBufferEnd()) {};

std::span<const Token> Lexer::lex() {
  handleIndentation();

  while (auto current_char = *advance()) {
    switch (current_char) {
    case '+':
      addToken(TokenType::PLUS);
      break;
    case '-':
      if (isDigit(*peek())) {
        scanNumber();
      } else if (match('>')) {
        advance();
        addToken(TokenType::RARROW);
      } else {
        addToken(TokenType::MINUS);
      }
      break;
    case '*':
      addToken(TokenType::MULT);
      break;
    case '/':
      if (expect('/')) {
        advance();
        addToken(TokenType::DIV);
      } else {
        addToken(TokenType::INVALID);
      }
      break;
    case '%':
      addToken(TokenType::MOD);
      break;
    case '=':
      if (match('=')) {
        advance();
        addToken(TokenType::EQUAL);
      } else {
        addToken(TokenType::ASSIGN);
      }
      break;
    case '!':
      if (expect('=')) {
        advance();
        addToken(TokenType::NEQUAL);
      } else {
        addToken(TokenType::INVALID);
      }
      break;
    case '<':
      if (match('=')) {
        advance();
        addToken(TokenType::LESSEQ);
      } else {
        addToken(TokenType::LESS);
      }
      break;
    case '>':
      if (match('=')) {
        advance();
        addToken(TokenType::GREATEQ);
      } else {
        addToken(TokenType::GREAT);
      }
      break;
    case '.':
      addToken(TokenType::DOT);
      break;
    case '[':
      addToken(TokenType::OPENBRACK);
      break;
    case ']':
      addToken(TokenType::CLOSEBRACK);
      break;
    case ',':
      addToken(TokenType::COMMA);
      break;
    case ':':
      addToken(TokenType::COLON);
      break;
    case '(':
      addToken(TokenType::OPENPAREN);
      break;
    case ')':
      addToken(TokenType::CLOSEPAREN);
      break;
    case '\r':
      std::ignore = match('\n');
      advance();
      [[fallthrough]];
    case '\n':
      handleNewLine();
      break;
    case '#':
      skipComment();
      break;
    case '"':
      scanString();
      break;
    case '\t':
      [[fallthrough]];
    case ' ':
      skipWhitespace();
      break;
    default:
      if (isAlpha(current_char) || current_char == '_') {
        scanIdOrKeyword();
      } else if (isDigit(current_char)) {
        scanNumber();
      } else {
        m_diag_manager.addError("unexpected character", getCurrentLexemeLocation());
        addToken(TokenType::INVALID);
      }
    }
  }

  while (m_indentation_levels.top() > 0) {
    addToken(TokenType::DEDENT);
    m_indentation_levels.pop();
  }

  return m_tokens;
}

std::optional<char> Lexer::peek(const int n) const {
  if (m_lexeme_start + m_lexeme_length + n > m_buffer_end) {
    return std::nullopt;
  }

  return *(m_lexeme_start + m_lexeme_length + n);
}

std::optional<char> Lexer::advance() {
  const auto current_char = peek();

  if (current_char) {
    ++m_lexeme_length;
  }

  return current_char;
}

bool Lexer::expect(const char ch) {
  if (match(ch)) {
    return true;
  }

  m_diag_manager.addError(formatv("unexpected character. Did you mean `{0}{1}`?",
                    getCurrentLexeme(), ch), getCurrentLexemeLocation());

  return false;
}

void Lexer::addToken(TokenType type) {
  m_tokens.emplace_back(type, getCurrentLexeme(), getCurrentLexemeLocation());

  m_lexeme_start += m_lexeme_length;
  m_lexeme_length = 0;
}

void Lexer::skipComment() {
  while (!match('\r', '\n', '\0')) { advance(); };

  m_lexeme_start += m_lexeme_length;
  m_lexeme_length = 0;
}

void Lexer::skipWhitespace() {
  while (match(' ', '\t')) { advance(); };

  m_lexeme_start += m_lexeme_length;
  m_lexeme_length = 0;
}

void Lexer::scanNumber() {
  TokenType token_type = TokenType::INTLIT;

  while (isDigit(*peek())) {
    advance();
  }

  StringRef value = getCurrentLexeme();

  if ((value.starts_with('0') && value.size() > 1) || (value.starts_with("-0") && value.size() > 2)) {
    token_type = TokenType::INVALID;
    m_diag_manager.addError("an integer may not have leading zeros", getCurrentLexemeLocation());
  }

  std::int32_t value_as_int;
  if (!to_integer(value, value_as_int, 10)) {
    token_type = TokenType::INVALID;
    m_diag_manager.addError("an integer must be within the range [-2147483648, 2147483647]", getCurrentLexemeLocation());
  }

  addToken(token_type);
}

void Lexer::scanIdOrKeyword() {
  while (isAlnum(*peek()) || *peek() == '_') {
    advance();
  }

  StringRef value = getCurrentLexeme();

  addToken(keywords.contains(value) ? keywords.at(value) : TokenType::ID);
}

void Lexer::scanString() {
  auto token_type = isDigit(*peek()) ? TokenType::STRING : TokenType::IDSTRING;

  while (!match('"', '\0', '\r', '\n')) {
    if (token_type != TokenType::INVALID && !isAlnum(*peek()) && *peek() != '_') {
      token_type = TokenType::STRING;
    }

    if (match('\\')) {
      advance();
      
      if (!match('\\', 'n', 't', '"')) {
        token_type = TokenType::INVALID;
        SMRange location = {getCurrentLexemeEndLocation(), getCurrentLexemeEndLocation()};
        m_diag_manager.addError("invalid escape character. Only \\\\, \\\", \\n and \\t are allowed", location);
      }
    }

    advance();
  }

  if (!match('"')) {
    token_type = TokenType::INVALID;
    SMFixIt fixit{getCurrentLexemeLocation(), getCurrentLexeme().str() + '"'};
    m_diag_manager.addError("unterminated string", getCurrentLexemeLocation(), fixit);
  }

  advance();

  addToken(token_type);
}

void Lexer::handleNewLine() {
  if (!m_is_blank_line) {
    addToken(TokenType::NEWLINE);
  } else {
    m_lexeme_start += m_lexeme_length;
    m_lexeme_length = 0;
  }

  // Reset whether the next line is a blank line. If the next line is not a blank line, it will be handled by `handleIndentation`.
  m_is_blank_line = true;

  // Handle indentation for the next line. We do this after encountering a new line so that logical lines without any leading whitespace will be handled.
  // Otherwise, we would have to do this when we encounter any first character in a line.
  handleIndentation();
}

void Lexer::handleIndentation() {
  while (match(' ', '\t')) { advance(); }

  std::size_t indentation_level = m_lexeme_length;

  // Indentation should only be handled for logical lines. A logical line is a line that does not solely consist of whitespace or a comment.
  // Therefore, if the first character we encounter is a #, it means it is only whitespace with a comment.
  // If the first character is a new line, it is only whitespace.
  if (match('#', '\r', '\n')) {
    return;
  }

  m_is_blank_line = false;

  if (indentation_level > m_indentation_levels.top()) {
    addToken(TokenType::INDENT);
    m_indentation_levels.push(indentation_level);
  } else {
    while (indentation_level < m_indentation_levels.top()) {
      addToken(TokenType::DEDENT);
      m_indentation_levels.pop();
    }
  }
}

StringRef Lexer::getCurrentLexeme() const {
  return StringRef(m_lexeme_start, m_lexeme_length);
}

SMLoc Lexer::getCurrentLexemeStartLocation() const {
  return SMLoc::getFromPointer(m_lexeme_start);
}

SMLoc Lexer::getCurrentLexemeEndLocation() const {
  return SMLoc::getFromPointer(m_lexeme_start + m_lexeme_length);
}

SMRange Lexer::getCurrentLexemeLocation() const {
  return {getCurrentLexemeStartLocation(), getCurrentLexemeEndLocation()};
}
}