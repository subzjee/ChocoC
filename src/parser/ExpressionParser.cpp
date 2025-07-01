#include "parser/ExpressionParser.h"
#include "ast/BinaryExpression.h"
#include "ast/ConstantExpression.h"
#include "ast/Identifier.h"
#include "ast/Literal.h"
#include "parser/ExpressionKind.h"
#include <llvm-21/llvm/Support/raw_ostream.h>

namespace chocopy {
[[nodiscard]] std::unique_ptr<ast::Expression>
ExpressionParser::parseExpression(unsigned int min_power) {
  auto lhs = parsePrefix();

  if (!lhs) {
    return nullptr;
  }

  while (const auto token = m_token_stream.peek()) {
    const auto binding_power = getInfixPower(token->get().getType());

    if (!binding_power || binding_power->first < min_power) {
      break;
    }

    if (m_token_stream.match(TokenType::AND, TokenType::OR)) {
      m_token_stream.advance();

      auto rhs = parseExpression(binding_power->second);

      if (!rhs) {
        m_diag_manager.addError("expected an expression",
                                m_token_stream.peek()->get().getLocation());
        return nullptr;
      }

      lhs = std::make_unique<ast::BinaryExpression<ast::Expression>>(std::move(lhs), token->get(), std::move(rhs));

    } else if (token->get().isBinOp()) {
      m_token_stream.advance();

      auto rhs = parseExpression(binding_power->second);

      if (!rhs) {
        m_diag_manager.addError("expected a constant expression",
                                m_token_stream.peek()->get().getLocation());
        return nullptr;
      }

      if (!dynamic_cast<ast::ConstantExpression *>(lhs.get())) {
        m_diag_manager.addError("expected a constant expression",
                                lhs->getLocation());
        return nullptr;
      }

      if (!dynamic_cast<ast::ConstantExpression *>(rhs.get())) {
        m_diag_manager.addError("expected a constant expression",
                                rhs->getLocation());
        return nullptr;
      }

      lhs = std::make_unique<ast::BinaryExpression<ast::ConstantExpression>>(
          std::unique_ptr<ast::ConstantExpression>(
              static_cast<ast::ConstantExpression *>(lhs.release())),
          token->get(),
          std::unique_ptr<ast::ConstantExpression>(
              static_cast<ast::ConstantExpression *>(rhs.release())));
    }
  }

  return lhs;
}

[[nodiscard]] std::unique_ptr<ast::Expression> ExpressionParser::parsePrefix() {
  const auto token = m_token_stream.peek();

  if (!token) {
    return nullptr;
  }

  if (m_token_stream.match(TokenType::STRING, TokenType::IDSTRING,
                           TokenType::FALSE, TokenType::TRUE, TokenType::INTLIT,
                           TokenType::NONE)) {
    m_token_stream.advance();
    return std::make_unique<ast::Literal>(*token);
  } else if (m_token_stream.match(TokenType::ID)) {
    m_token_stream.advance();
    return std::make_unique<ast::Identifier>(*token);
  } else if (m_token_stream.match(TokenType::OPENPAREN)) {
    m_token_stream.advance();
    auto expr = parseExpression(0);

    if (!m_token_stream.match(TokenType::CLOSEPAREN)) {
      m_diag_manager.addError("expected `)`",
                              m_token_stream.peek()->get().getLocation());
      return nullptr;
    }
    m_token_stream.advance();

    return expr;
  }

  return nullptr;
}

[[nodiscard]] BindingPower ExpressionParser::getPrefixPower(TokenType op) {
  switch (op) {
  case TokenType::NOT:
    return {{40, 40}};
  case TokenType::MINUS:
    return {{80, 80}};
  default:
    return std::nullopt;
  }
}

[[nodiscard]] BindingPower ExpressionParser::getInfixPower(TokenType op) {
  switch (op) {
  case TokenType::OR:
    return {{20, 21}};
  case TokenType::AND:
    return {{30, 31}};
  case TokenType::EQUAL:
    [[fallthrough]];
  case TokenType::NEQUAL:
    [[fallthrough]];
  case TokenType::LESS:
    [[fallthrough]];
  case TokenType::GREAT:
    [[fallthrough]];
  case TokenType::LESSEQ:
    [[fallthrough]];
  case TokenType::GREATEQ:
    [[fallthrough]];
  case TokenType::IS:
    return {{50, 50}};
  case TokenType::PLUS:
    [[fallthrough]];
  case TokenType::MINUS:
    return {{60, 61}};
  case TokenType::MULT:
    [[fallthrough]];
  case TokenType::DIV:
    [[fallthrough]];
  case TokenType::MOD:
    return {{70, 71}};
  default:
    return std::nullopt;
  }
}
} // namespace chocopy