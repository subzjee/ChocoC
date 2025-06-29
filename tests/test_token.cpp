#include "lexer/Token.h"

#include "gtest/gtest.h"

#include <array>
#include <utility>

using namespace chocopy;

TEST(TokenTest, TokenTypeToString) {
  static const std::array<std::pair<TokenType, std::string>, 61> mapping = {
      std::make_pair(TokenType::AS, "AS"),
      std::make_pair(TokenType::ASSERT, "ASSERT"),
      std::make_pair(TokenType::ASYNC, "ASYNC"),
      std::make_pair(TokenType::AWAIT, "AWAIT"),
      std::make_pair(TokenType::BREAK, "BREAK"),
      std::make_pair(TokenType::CLASS, "CLASS"),
      std::make_pair(TokenType::CONTINUE, "CONTINUE"),
      std::make_pair(TokenType::DEF, "DEF"),
      std::make_pair(TokenType::DEL, "DEL"),
      std::make_pair(TokenType::ELIF, "ELIF"),
      std::make_pair(TokenType::ELSE, "ELSE"),
      std::make_pair(TokenType::EXCEPT, "EXCEPT"),
      std::make_pair(TokenType::FALSE, "FALSE"),
      std::make_pair(TokenType::FINALLY, "FINALLY"),
      std::make_pair(TokenType::FOR, "FOR"),
      std::make_pair(TokenType::FROM, "FROM"),
      std::make_pair(TokenType::GLOBAL, "GLOBAL"),
      std::make_pair(TokenType::IF, "IF"),
      std::make_pair(TokenType::IMPORT, "IMPORT"),
      std::make_pair(TokenType::IN, "IN"),
      std::make_pair(TokenType::IS, "IS"),
      std::make_pair(TokenType::LAMBDA, "LAMBDA"),
      std::make_pair(TokenType::NONE, "NONE"),
      std::make_pair(TokenType::NONLOCAL, "NONLOCAL"),
      std::make_pair(TokenType::NOT, "NOT"),
      std::make_pair(TokenType::OR, "OR"),
      std::make_pair(TokenType::PASS, "PASS"),
      std::make_pair(TokenType::RAISE, "RAISE"),
      std::make_pair(TokenType::RETURN, "RETURN"),
      std::make_pair(TokenType::TRUE, "TRUE"),
      std::make_pair(TokenType::TRY, "TRY"),
      std::make_pair(TokenType::WHILE, "WHILE"),
      std::make_pair(TokenType::WITH, "WITH"),
      std::make_pair(TokenType::YIELD, "YIELD"),
      std::make_pair(TokenType::PLUS, "PLUS"),
      std::make_pair(TokenType::MINUS, "MINUS"),
      std::make_pair(TokenType::MULT, "MULT"),
      std::make_pair(TokenType::DIV, "DIV"),
      std::make_pair(TokenType::MOD, "MOD"),
      std::make_pair(TokenType::EQUAL, "EQUAL"),
      std::make_pair(TokenType::NEQUAL, "NEQUAL"),
      std::make_pair(TokenType::LESS, "LESS"),
      std::make_pair(TokenType::GREAT, "GREAT"),
      std::make_pair(TokenType::LESSEQ, "LESSEQ"),
      std::make_pair(TokenType::GREATEQ, "GREATEQ"),
      std::make_pair(TokenType::DOT, "DOT"),
      std::make_pair(TokenType::OPENBRACK, "OPENBRACK"),
      std::make_pair(TokenType::CLOSEBRACK, "CLOSEBRACK"),
      std::make_pair(TokenType::ASSIGN, "ASSIGN"),
      std::make_pair(TokenType::COMMA, "COMMA"),
      std::make_pair(TokenType::COLON, "COLON"),
      std::make_pair(TokenType::OPENPAREN, "OPENPAREN"),
      std::make_pair(TokenType::CLOSEPAREN, "CLOSEPAREN"),
      std::make_pair(TokenType::RARROW, "RARROW"),
      std::make_pair(TokenType::INTLIT, "INTLIT"),
      std::make_pair(TokenType::STRING, "STRING"),
      std::make_pair(TokenType::IDSTRING, "IDSTRING"),
      std::make_pair(TokenType::NEWLINE, "NEWLINE"),
      std::make_pair(TokenType::INDENT, "INDENT"),
      std::make_pair(TokenType::DEDENT, "DEDENT"),
      std::make_pair(TokenType::ID, "ID")};

  for (const auto &[token_type, expected_string] : mapping) {
    EXPECT_EQ(toString(token_type), expected_string);
  }
}