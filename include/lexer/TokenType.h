#pragma once

#include "llvm/ADT/StringMap.h"

namespace chocopy {
#define TOKEN_TYPES                                                            \
  TOKEN_TYPE(INVALID)                                                          \
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

enum class TokenType {
#define TOKEN_TYPE(name) name,
  TOKEN_TYPES
#undef TOKEN_TYPE
};

/// Get the string representation of an ErrorType.
/// @param type The TokenType to get the string representation of.
/// @returns The string representation of the TokenType.
constexpr std::string toString(TokenType type) {
  switch (type) {
#define TOKEN_TYPE(name)                                                       \
  case TokenType::name:                                                        \
    return #name;
    TOKEN_TYPES
#undef TOKEN_TYPE
  }
}

const llvm::StringMap<TokenType> keywords = {{"and", TokenType::AND},
                                             {"as", TokenType::AS},
                                             {"assert", TokenType::ASSERT},
                                             {"async", TokenType::ASYNC},
                                             {"await", TokenType::AWAIT},
                                             {"break", TokenType::BREAK},
                                             {"class", TokenType::CLASS},
                                             {"continue", TokenType::CONTINUE},
                                             {"def", TokenType::DEF},
                                             {"del", TokenType::DEL},
                                             {"elif", TokenType::ELIF},
                                             {"else", TokenType::ELSE},
                                             {"except", TokenType::EXCEPT},
                                             {"False", TokenType::FALSE},
                                             {"finally", TokenType::FINALLY},
                                             {"for", TokenType::FOR},
                                             {"from", TokenType::FROM},
                                             {"global", TokenType::GLOBAL},
                                             {"if", TokenType::IF},
                                             {"import", TokenType::IMPORT},
                                             {"in", TokenType::IN},
                                             {"is", TokenType::IS},
                                             {"lambda", TokenType::LAMBDA},
                                             {"None", TokenType::NONE},
                                             {"nonlocal", TokenType::NONLOCAL},
                                             {"not", TokenType::NOT},
                                             {"or", TokenType::OR},
                                             {"pass", TokenType::PASS},
                                             {"raise", TokenType::RAISE},
                                             {"return", TokenType::RETURN},
                                             {"True", TokenType::TRUE},
                                             {"try", TokenType::TRY},
                                             {"while", TokenType::WHILE},
                                             {"with", TokenType::WITH},
                                             {"yield", TokenType::YIELD}};
} // namespace chocopy