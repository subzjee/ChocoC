#include "lexer/Lexer.h"

#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/SourceMgr.h"
#include "gtest/gtest.h"

using namespace llvm;
using namespace chocopy;

class LexerTest : public testing::Test {
protected:
  SourceMgr source_manager;
};

TEST_F(LexerTest, KeywordRecognition) {
  DiagnosticsManager diagnostics_manager{source_manager};
  auto file = MemoryBuffer::getFile("tests/input/lexer/keywords.chocopy");
  unsigned int buffer_id =
      source_manager.AddNewSourceBuffer(std::move(file.get()), SMLoc());
  Lexer lexer{buffer_id, source_manager, diagnostics_manager};

  constexpr std::array<TokenType, 35> expected_token_types = {
      TokenType::AND,      TokenType::AS,       TokenType::ASSERT,
      TokenType::ASYNC,    TokenType::AWAIT,    TokenType::BREAK,
      TokenType::CLASS,    TokenType::CONTINUE, TokenType::DEF,
      TokenType::DEL,      TokenType::ELIF,     TokenType::ELSE,
      TokenType::EXCEPT,   TokenType::FALSE,    TokenType::FINALLY,
      TokenType::FOR,      TokenType::FROM,     TokenType::GLOBAL,
      TokenType::IF,       TokenType::IMPORT,   TokenType::IN,
      TokenType::IS,       TokenType::LAMBDA,   TokenType::NONE,
      TokenType::NONLOCAL, TokenType::NOT,      TokenType::OR,
      TokenType::PASS,     TokenType::RAISE,    TokenType::RETURN,
      TokenType::TRUE,     TokenType::TRY,      TokenType::WHILE,
      TokenType::WITH,     TokenType::YIELD};

  // Filter out NEWLINE tokens because they are only there for readability in the test file.
  const auto filtered_tokens =
      make_filter_range(lexer.lex(), [](const Token& token) {
        return token.getType() != TokenType::NEWLINE;
      });

  for (const auto& [token, expected_type] :
       zip_equal(filtered_tokens, expected_token_types)) {
    EXPECT_EQ(token.getType(), expected_type);
  }
}

TEST_F(LexerTest, OperatorRecognition) {
  DiagnosticsManager diagnostics_manager{source_manager};
  auto file = MemoryBuffer::getFile("tests/input/lexer/operators.chocopy");
  unsigned int buffer_id =
      source_manager.AddNewSourceBuffer(std::move(file.get()), SMLoc());
  Lexer lexer{buffer_id, source_manager, diagnostics_manager};

  constexpr std::array<TokenType, 20> expected_token_types = {
      TokenType::PLUS,       TokenType::MINUS,      TokenType::MULT,
      TokenType::DIV,        TokenType::MOD,        TokenType::LESS,
      TokenType::GREAT,      TokenType::LESSEQ,     TokenType::GREATEQ,
      TokenType::EQUAL,      TokenType::NEQUAL,     TokenType::ASSIGN,
      TokenType::OPENPAREN,  TokenType::CLOSEPAREN, TokenType::OPENBRACK,
      TokenType::CLOSEBRACK, TokenType::COMMA,      TokenType::COLON,
      TokenType::DOT,        TokenType::RARROW};

  // Filter out NEWLINE tokens because they are only there for readability in the test file.
  const auto filtered_tokens =
      make_filter_range(lexer.lex(), [](const Token& token) {
        return token.getType() != TokenType::NEWLINE;
      });

  for (const auto& [token, expected_type] :
       zip_equal(filtered_tokens, expected_token_types)) {
    EXPECT_EQ(token.getType(), expected_type);
  }
}

TEST_F(LexerTest, IntegerLiterals) {
  DiagnosticsManager diagnostics_manager{source_manager};
  auto file = MemoryBuffer::getFile("tests/input/lexer/integer_literals.chocopy");
  unsigned int buffer_id =
      source_manager.AddNewSourceBuffer(std::move(file.get()), SMLoc());
  Lexer lexer{buffer_id, source_manager, diagnostics_manager};

  constexpr std::array<TokenType, 8> expected_token_types = {
      TokenType::INTLIT,  TokenType::INTLIT,  TokenType::INTLIT,
      TokenType::INVALID, TokenType::INVALID, TokenType::INVALID,
      TokenType::INVALID, TokenType::INVALID};

  // Filter out NEWLINE tokens because they are only there for readability in the test file.
  const auto filtered_tokens =
      make_filter_range(lexer.lex(), [](const Token& token) {
        return token.getType() != TokenType::NEWLINE;
      });

  for (const auto& [token, expected_type] :
       zip_equal(filtered_tokens, expected_token_types)) {
    EXPECT_EQ(token.getType(), expected_type);
  }
}

TEST_F(LexerTest, StringLiterals) {
  DiagnosticsManager diagnostics_manager{source_manager};
  auto file = MemoryBuffer::getFile("tests/input/lexer/string_literals.chocopy");
  unsigned int buffer_id =
      source_manager.AddNewSourceBuffer(std::move(file.get()), SMLoc());
  Lexer lexer{buffer_id, source_manager, diagnostics_manager};

  constexpr std::array<TokenType, 8> expected_token_types = {
      TokenType::IDSTRING, TokenType::STRING, TokenType::STRING,
      TokenType::STRING,   TokenType::STRING, TokenType::STRING,
      TokenType::INVALID,  TokenType::INVALID};

  // Filter out NEWLINE tokens because they are only there for readability in the test file.
  const auto filtered_tokens =
      make_filter_range(lexer.lex(), [](const Token& token) {
        return token.getType() != TokenType::NEWLINE;
      });

  for (const auto& [token, expected_type] :
       zip_equal(filtered_tokens, expected_token_types)) {
    EXPECT_EQ(token.getType(), expected_type);
  }
}

TEST_F(LexerTest, Indentation) {
  DiagnosticsManager diagnostics_manager{source_manager};
  auto file = MemoryBuffer::getFile("tests/input/lexer/indentation.chocopy");
  unsigned int buffer_id =
      source_manager.AddNewSourceBuffer(std::move(file.get()), SMLoc());
  Lexer lexer{buffer_id, source_manager, diagnostics_manager};

  constexpr std::array<TokenType, 27> expected_token_types = {
      TokenType::ID,      TokenType::COLON,   TokenType::ID,
      TokenType::ASSIGN,  TokenType::INTLIT,  TokenType::NEWLINE,
      TokenType::INDENT,  TokenType::ID,      TokenType::ASSIGN,
      TokenType::INTLIT,  TokenType::NEWLINE, TokenType::INDENT,
      TokenType::ID,      TokenType::ASSIGN,  TokenType::INTLIT,
      TokenType::NEWLINE, TokenType::INDENT,  TokenType::ID,
      TokenType::ASSIGN,  TokenType::INTLIT,  TokenType::NEWLINE,
      TokenType::DEDENT,  TokenType::ID,      TokenType::ASSIGN,
      TokenType::INTLIT,  TokenType::DEDENT,  TokenType::DEDENT};

  const auto tokens = lexer.lex();

  for (const auto& [token, expected_type] :
       zip_equal(tokens, expected_token_types)) {
    EXPECT_EQ(token.getType(), expected_type);
  }
}

TEST_F(LexerTest, UnexpectedCharacters) {
  DiagnosticsManager diagnostics_manager{source_manager};

  auto file = MemoryBuffer::getFile("tests/input/lexer/unexpected_character.chocopy");
  unsigned int buffer_id =
      source_manager.AddNewSourceBuffer(std::move(file.get()), SMLoc());
  Lexer lexer{buffer_id, source_manager, diagnostics_manager};

  constexpr std::array<TokenType, 12> expected_token_types = {
      TokenType::INVALID, TokenType::INVALID, TokenType::INVALID,
      TokenType::INVALID, TokenType::INVALID, TokenType::INVALID,
      TokenType::INVALID, TokenType::INVALID, TokenType::INVALID,
      TokenType::INVALID, TokenType::INVALID, TokenType::INVALID};

  // Filter out NEWLINE tokens because they are only there for readability in the test file.
  const auto filtered_tokens =
  make_filter_range(lexer.lex(), [](const Token& token) {
    return token.getType() != TokenType::NEWLINE;
  });

  for (const auto& [token, expected_type] :
       zip_equal(filtered_tokens, expected_token_types)) {
    EXPECT_EQ(token.getType(), expected_type);
  }
}