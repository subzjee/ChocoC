#pragma once

#include "TokenStream.h"
#include "ast/Expression.h"
#include "lexer/TokenType.h"
#include "parser/ExpressionKind.h"

#include <memory>
#include <optional>
#include <utility>

namespace chocopy {
using BindingPower = std::optional<std::pair<unsigned int, unsigned int>>;

class ExpressionParser {
public:
  ExpressionParser(TokenStream& token_stream) : m_token_stream(token_stream) {}

  /// Parse an expression,
  /// @param min_power The minimum binding power,
  /// @param expression_kind The kind of expression to parse.
  /// @returns The expression.
  [[nodiscard]] std::unique_ptr<ast::Expression> parseExpression(int min_power = 0, ExpressionKind expression_kind = ExpressionKind::Normal);

private:
  /// Parse a prefix.
  /// @param expression_kind The kind of expression.
  /// @returns The prefix expression.
  [[nodiscard]] std::unique_ptr<ast::Expression> parsePrefix(ExpressionKind expression_kind);

  /// Get the prefix binding power of an operator within a specified kind of expression.
  /// @param op The operator.
  /// @param expression_kind The kind of expression.
  /// @returns The left and right binding power as a pair. For prefix powers, the left and right are equal.
  [[nodiscard]] BindingPower getPrefixPower(TokenType op, ExpressionKind expression_kind);

  /// Get the infix binding power of an operator within a specified kind of expression.
  /// @param op The operator.
  /// @param expression_kind The kind of expression.
  /// @returns The left and right binding power as a pair.
  [[nodiscard]] BindingPower getInfixPower(TokenType op, ExpressionKind expression_kind);

  TokenStream& m_token_stream;
};
}