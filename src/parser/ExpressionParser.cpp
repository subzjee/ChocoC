#include "parser/ExpressionParser.h"
#include "ast/BinaryExpression.h"
#include "ast/ConstantExpression.h"
#include "ast/GroupingExpression.h"
#include "ast/Identifier.h"
#include "ast/Literal.h"
#include "ast/UnaryExpression.h"

#include <memory>

namespace chocopy {
[[nodiscard]] std::unique_ptr<ast::Expression>
ExpressionParser::parseExpression(unsigned int min_power) {
  auto lhs = parsePrefix();

  if (!lhs) {
    return nullptr;
  }

  while (const auto token = m_token_stream.peek()) {
    const auto token_type = token->get().getType();

    if (!token->get().isBinOp()) {
      break;
    }

    const auto binding_power = getInfixPower(token_type);

    if (!binding_power || binding_power->first < min_power) {
      break;
    }

    m_token_stream.advance();

    auto rhs = parseExpression(binding_power->second);

    if (!rhs) {
      return nullptr;
    }

    // `AND` and `OR` are the only binary operators that have expressions
    // instead of constant expressions as their operands. All constant
    // expressions are expressions but not vice versa. Therefore, if the
    // operator is either of these, we don't have to check whether the operands
    // are constant expressions.
    if (token_type == TokenType::AND || token_type == TokenType::OR) {
      lhs = std::make_unique<ast::BinaryExpression<ast::Expression>>(
          std::move(lhs), token->get(), std::move(rhs));
      continue;
    }

    if (!dynamic_cast<ast::ConstantExpression*>(lhs.get())) {
      m_diag_manager.addError("expected a constant expression",
                              lhs.get()->getLocation());
      return nullptr;
    }

    if (!dynamic_cast<ast::ConstantExpression*>(rhs.get())) {
      m_diag_manager.addError("expected a constant expression",
                              rhs.get()->getLocation());
      return nullptr;
    }

    lhs = std::make_unique<ast::BinaryExpression<ast::ConstantExpression>>(
        std::unique_ptr<ast::ConstantExpression>(
            static_cast<ast::ConstantExpression*>(lhs.release())),
        token->get(),
        std::unique_ptr<ast::ConstantExpression>(
            static_cast<ast::ConstantExpression*>(rhs.release())));
  }

  return lhs;
}

[[nodiscard]] std::unique_ptr<ast::Expression> ExpressionParser::parsePrefix() {
  const auto token = m_token_stream.peek();

  if (!token) {
    return nullptr;
  }

  if (expect(TokenType::STRING, TokenType::IDSTRING, TokenType::FALSE,
             TokenType::TRUE, TokenType::INTLIT, TokenType::NONE)) {
    return std::make_unique<ast::Literal>(*token);
  } else if (expect(TokenType::ID)) {
    return std::make_unique<ast::Identifier>(*token);
  } else if (expect(TokenType::OPENPAREN)) {
    auto left_paren = m_token_stream.peek(-1);
    auto expr = parseExpression(0);

    if (!expect(TokenType::CLOSEPAREN)) {
      m_diag_manager.addError("unclosed '('", token->get().getLocation());
      return nullptr;
    }
    auto right_paren = m_token_stream.peek(-1);

    return std::make_unique<ast::GroupingExpression>(
        left_paren->get(), std::move(expr), right_paren->get());
  } else if (expect(TokenType::MINUS)) {
    const auto& op = m_token_stream.peek(-1);
    auto rhs = parseExpression(getPrefixPower(op->get().getType())->second);

    if (!rhs || !dynamic_cast<ast::ConstantExpression*>(rhs.get())) {
      m_diag_manager.addError("missing operand for unary '-'",
                              op->get().getLocation());
      return nullptr;
    }

    return std::make_unique<ast::UnaryExpression<ast::ConstantExpression>>(
        op->get(), std::unique_ptr<ast::ConstantExpression>(
                       static_cast<ast::ConstantExpression*>(rhs.release())));
  } else if (expect(TokenType::NOT)) {
    const auto& op = m_token_stream.peek(-1);
    auto rhs = parseExpression(getPrefixPower(op->get().getType())->second);

    if (!rhs) {
      m_diag_manager.addError("missing operand for 'not'",
                              op->get().getLocation());
      return nullptr;
    }

    return std::make_unique<ast::UnaryExpression<ast::Expression>>(
        op->get(), std::move(rhs));
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