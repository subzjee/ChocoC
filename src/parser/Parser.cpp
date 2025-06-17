#include "parser/Parser.h"
#include "parser/ParseContext.h"
#include "lexer/Token.h"
#include "lexer/TokenType.h"

#include "llvm/Support/FormatVariadic.h"

using namespace llvm;

namespace chocopy {
std::optional<std::reference_wrapper<const Token>> Parser::peek(const int n) const {
  const auto idx = m_current_idx + n;

  if (idx < 0 || idx >= m_tokens.size()) {
    return std::nullopt;
  }

  return std::cref(m_tokens[idx]);
}

std::optional<const Token> Parser::advance() {
  const auto current_token = peek();

  if (current_token) {
    ++m_current_idx;
  }

  return current_token;
}

std::unique_ptr<ProgramContext> Parser::parseProgram() {
  ProgramChildren children{};

  while (true) {
    if (peek() == TokenType::ID && peek(1) == TokenType::COLON) {
      auto var_def_ctx = parseVarDef();

      if (!var_def_ctx) {
        return nullptr;
      }

      children.push_back(std::move(var_def_ctx));
    } else {
      break;
    }
  }

  return std::make_unique<ProgramContext>(std::move(children));
}

std::unique_ptr<TypeContext> Parser::parseType() {
  if (match(TokenType::ID, TokenType::IDSTRING)) {
    return std::make_unique<TypeContext>(*peek(-1));
  } else if (match(TokenType::OPENBRACK)) {
    auto type_ctx = parseType();

    if (!type_ctx) {
      return nullptr;
    }

    if (!match(TokenType::CLOSEBRACK)) {
      m_diag_manager.addError(formatv("expected `]`"), peek()->get().getLocation());
      return nullptr;
    }

    return std::make_unique<TypeContext>(type_ctx->getBaseType(), type_ctx->getDimension() + 1);
  }

  m_diag_manager.addError(formatv("expected a type"), peek()->get().getLocation());

  return nullptr;
}

std::unique_ptr<TypedVarContext> Parser::parseTypedVar() {
  if (!match(TokenType::ID)) [[unlikely]] {
    m_diag_manager.addError(formatv("expected an identifier"), peek()->get().getLocation());
    return nullptr;
  }

  const auto& name = peek(-1);

  if (!match(TokenType::COLON)) [[unlikely]] {
    m_diag_manager.addError(formatv("expected `:`"), peek()->get().getLocation());
    return nullptr;
  }

  auto type_ctx = parseType();
  if (!type_ctx) {
    return nullptr;
  }

  return std::make_unique<TypedVarContext>(*name, type_ctx);
}

std::unique_ptr<VarDefContext> Parser::parseVarDef() {
  auto typed_var_ctx = parseTypedVar();
  if (!typed_var_ctx) {
    return nullptr;
  }

  if (!match(TokenType::ASSIGN)) {
    m_diag_manager.addError(formatv("expected `=`"), peek()->get().getLocation());
    return nullptr;
  }

  auto literal_ctx = parseLiteral();
  if (!literal_ctx) {
    return nullptr;
  }

  return std::make_unique<VarDefContext>(typed_var_ctx, literal_ctx);
}

std::unique_ptr<LiteralContext> Parser::parseLiteral() {
  if (!match(TokenType::NONE, TokenType::FALSE, TokenType::TRUE, TokenType::INTLIT, TokenType::IDSTRING, TokenType::STRING)) {
    m_diag_manager.addError(formatv("expected a literal value"), peek()->get().getLocation());
    return nullptr;
  }

  return std::make_unique<LiteralContext>(*peek(-1));
}
} // namespace chocopy