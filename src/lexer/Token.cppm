export module Token;

import <cstddef>;
import <format>;
import <string>;

#define TOKEN_TYPES                                                            \
  TOKEN_TYPE(AND)                                                              \
  TOKEN_TYPE(AS)                                                               \
  TOKEN_TYPE(ASSERT)                                                           \
  TOKEN_TYPE(ASYNC)                                                            \
  TOKEN_TYPE(AWAIT)                                                            \
  TOKEN_TYPE(BREAK)                                                            \
  TOKEN_TYPE(CLASS)                                                            \
  TOKEN_TYPE(CONTINUE)                                                         \
  TOKEN_TYPE(DEF)                                                              \
  TOKEN_TYPE(DEL)                                                              \
  TOKEN_TYPE(ELIF)                                                             \
  TOKEN_TYPE(ELSE)                                                             \
  TOKEN_TYPE(EXCEPT)                                                           \
  TOKEN_TYPE(FALSE)                                                            \
  TOKEN_TYPE(FINALLY)                                                          \
  TOKEN_TYPE(FOR)                                                              \
  TOKEN_TYPE(FROM)                                                             \
  TOKEN_TYPE(GLOBAL)                                                           \
  TOKEN_TYPE(IF)                                                               \
  TOKEN_TYPE(IMPORT)                                                           \
  TOKEN_TYPE(IN)                                                               \
  TOKEN_TYPE(IS)                                                               \
  TOKEN_TYPE(LAMBDA)                                                           \
  TOKEN_TYPE(NONE)                                                             \
  TOKEN_TYPE(NONLOCAL)                                                         \
  TOKEN_TYPE(NOT)                                                              \
  TOKEN_TYPE(OR)                                                               \
  TOKEN_TYPE(PASS)                                                             \
  TOKEN_TYPE(RAISE)                                                            \
  TOKEN_TYPE(RETURN)                                                           \
  TOKEN_TYPE(TRUE)                                                             \
  TOKEN_TYPE(TRY)                                                              \
  TOKEN_TYPE(WHILE)                                                            \
  TOKEN_TYPE(WITH)                                                             \
  TOKEN_TYPE(YIELD)                                                            \
  TOKEN_TYPE(PLUS)                                                             \
  TOKEN_TYPE(MINUS)                                                            \
  TOKEN_TYPE(MULT)                                                             \
  TOKEN_TYPE(DIV)                                                              \
  TOKEN_TYPE(MOD)                                                              \
  TOKEN_TYPE(EQUAL)                                                            \
  TOKEN_TYPE(NEQUAL)                                                           \
  TOKEN_TYPE(LESS)                                                             \
  TOKEN_TYPE(GREAT)                                                            \
  TOKEN_TYPE(LESSEQ)                                                           \
  TOKEN_TYPE(GREATEQ)                                                          \
  TOKEN_TYPE(DOT)                                                              \
  TOKEN_TYPE(OPENBRACK)                                                        \
  TOKEN_TYPE(CLOSEBRACK)                                                       \
  TOKEN_TYPE(ASSIGN)                                                           \
  TOKEN_TYPE(COMMA)                                                            \
  TOKEN_TYPE(COLON)                                                            \
  TOKEN_TYPE(OPENPAREN)                                                        \
  TOKEN_TYPE(CLOSEPAREN)                                                       \
  TOKEN_TYPE(RARROW)                                                           \
  TOKEN_TYPE(INTLIT)                                                           \
  TOKEN_TYPE(STRING)                                                           \
  TOKEN_TYPE(IDSTRING)                                                         \
  TOKEN_TYPE(NEWLINE)                                                          \
  TOKEN_TYPE(INDENT)                                                           \
  TOKEN_TYPE(DEDENT)                                                           \
  TOKEN_TYPE(ID)

export enum class TokenType {
#define TOKEN_TYPE(name) name,
  TOKEN_TYPES
#undef TOKEN_TYPE
};

export std::string to_string(const TokenType &token) {
  switch (token) {
#define TOKEN_TYPE(name)                                                       \
  case TokenType::name:                                                        \
    return #name;
    TOKEN_TYPES
#undef TOKEN_TYPE
  default:
    return "UNKNOWN";
  }
}

export struct Token {
  Token(TokenType type, std::string_view text, std::size_t line_no = 0,
        std::size_t col_no = 0)
      : type{type}, text{text}, line_no{line_no}, col_no{col_no} {};

  [[nodiscard]] std::string to_string() const {
    return std::format("[{}:{}] {}, {}", line_no, col_no, ::to_string(type),
                       text);
  }

  const TokenType type;
  const std::string text;
  const std::size_t line_no;
  const std::size_t col_no;
};