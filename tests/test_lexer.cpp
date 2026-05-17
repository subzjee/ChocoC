#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/SourceMgr.h"
#include "gtest/gtest.h"

#include "lexer/Lexer.h"

#include "lexer/Token.h"

using namespace llvm;
using namespace chocopy;

class LexerTest : public testing::Test {
protected:
  SourceMgr source_manager;
  DiagnosticsManager diagnostics_manager{source_manager};
  std::unique_ptr<Lexer> lexer;

  std::span<const Token> lexFromFile(const std::string& path) {
    auto file = MemoryBuffer::getFile("tests/input/lexer/" + path);
    unsigned bufID =
        source_manager.AddNewSourceBuffer(std::move(file.get()), SMLoc());
    lexer = std::make_unique<Lexer>(bufID, source_manager, diagnostics_manager);
    return lexer->lex();
  }

  auto filterNewLines(const std::span<const Token>& tokens) {
    return make_filter_range(tokens, [](const Token& t) {
      return t.getType() != TokenType::NEWLINE;
    });
  }

  void TearDown() override { diagnostics_manager.clear(); }
};

TEST_F(LexerTest, KeywordRecognition) {
  const auto tokens = filterNewLines(lexFromFile("valid/keywords.choco"));

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

  for (const auto& [token, expected_type] :
       zip_equal(tokens, expected_token_types)) {
    EXPECT_EQ(token.getType(), expected_type);
  }

  EXPECT_FALSE(diagnostics_manager.hadError());
}

TEST_F(LexerTest, OperatorRecognition) {
  auto tokens = filterNewLines(lexFromFile("valid/operators.choco"));

  constexpr std::array<TokenType, 20> expected_token_types = {
      TokenType::PLUS,       TokenType::MINUS,      TokenType::MULT,
      TokenType::DIV,        TokenType::MOD,        TokenType::LESS,
      TokenType::GREAT,      TokenType::LESSEQ,     TokenType::GREATEQ,
      TokenType::EQUAL,      TokenType::NEQUAL,     TokenType::ASSIGN,
      TokenType::OPENPAREN,  TokenType::CLOSEPAREN, TokenType::OPENBRACK,
      TokenType::CLOSEBRACK, TokenType::COMMA,      TokenType::COLON,
      TokenType::DOT,        TokenType::RARROW};

  for (const auto& [token, expected_type] :
       zip_equal(tokens, expected_token_types)) {
    EXPECT_EQ(token.getType(), expected_type);
  }

  EXPECT_FALSE(diagnostics_manager.hadError());
}

TEST_F(LexerTest, ValidIntegerLiterals) {
  auto tokens = filterNewLines(lexFromFile("valid/integer_literals.choco"));

  constexpr std::array<TokenType, 2> expected_token_types = {TokenType::INTLIT,
                                                             TokenType::INTLIT};

  for (const auto& [token, expected_type] :
       zip_equal(tokens, expected_token_types)) {
    EXPECT_EQ(token.getType(), expected_type);
  }

  EXPECT_FALSE(diagnostics_manager.hadError());
}

TEST_F(LexerTest, InvalidIntegerLiterals) {
  auto tokens = filterNewLines(lexFromFile("invalid/integer_literals.choco"));

  constexpr std::array<TokenType, 3> expected_token_types = {
      TokenType::INVALID, TokenType::INVALID};

  for (const auto& [token, expected_type] :
       zip_equal(tokens, expected_token_types)) {
    EXPECT_EQ(token.getType(), expected_type);
  }

  ASSERT_TRUE(diagnostics_manager.hadError());
}

TEST_F(LexerTest, ValidStringLiterals) {
  auto tokens = filterNewLines(lexFromFile("valid/string_literals.choco"));

  constexpr std::array<TokenType, 6> expected_token_types = {
      TokenType::IDSTRING, TokenType::STRING, TokenType::STRING,
      TokenType::STRING,   TokenType::STRING, TokenType::STRING};

  for (const auto& [token, expected_type] :
       zip_equal(tokens, expected_token_types)) {
    EXPECT_EQ(token.getType(), expected_type);
  }

  EXPECT_FALSE(diagnostics_manager.hadError());
}

TEST_F(LexerTest, InvalidStringLiterals) {
  auto tokens = filterNewLines(lexFromFile("invalid/string_literals.choco"));

  constexpr std::array<TokenType, 2> expected_token_types = {
      TokenType::INVALID, TokenType::INVALID};

  for (const auto& [token, expected_type] :
       zip_equal(tokens, expected_token_types)) {
    EXPECT_EQ(token.getType(), expected_type);
  }

  ASSERT_TRUE(diagnostics_manager.hadError());
}

TEST_F(LexerTest, Indentation) {
  std::span<const Token> tokens = lexFromFile("valid/indentation.choco");

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

  for (const auto& [token, expected_type] :
       zip_equal(tokens, expected_token_types)) {
    EXPECT_EQ(token.getType(), expected_type);
  }

  EXPECT_FALSE(diagnostics_manager.hadError());
}

TEST_F(LexerTest, UnexpectedCharacters) {
  auto tokens =
      filterNewLines(lexFromFile("invalid/unexpected_character.choco"));

  constexpr std::array<TokenType, 12> expected_token_types = {
      TokenType::INVALID, TokenType::INVALID, TokenType::INVALID,
      TokenType::INVALID, TokenType::INVALID, TokenType::INVALID,
      TokenType::INVALID, TokenType::INVALID, TokenType::INVALID,
      TokenType::INVALID, TokenType::INVALID, TokenType::INVALID};

  for (const auto& [token, expected_type] :
       zip_equal(tokens, expected_token_types)) {
    EXPECT_EQ(token.getType(), expected_type);
  }

  ASSERT_TRUE(diagnostics_manager.hadError());
}
