#pragma once

#include "DiagnosticsManager.h"
#include "TokenStream.h"
#include "ast/BinaryExpression.h"
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
  explicit ExpressionParser(TokenStream& token_stream,
                            DiagnosticsManager& diagnostics_manager)
      : m_token_stream(token_stream), m_diag_manager(diagnostics_manager) {}

  /// Parse an expression,
  /// @param min_power The minimum binding power,
  /// @param expression_kind The kind of expression to parse.
  /// @returns The expression.
  [[nodiscard]] std::unique_ptr<ast::Expression>
  parseExpression(unsigned int min_power = 0);

private:
  /// Parse a prefix.
  /// @returns The prefix expression.
  [[nodiscard]] std::unique_ptr<ast::Expression> parsePrefix();

  /// Get the prefix binding power of an operator.
  /// @param op The operator.
  /// @returns The left and right binding power as a pair. For prefix powers,
  /// the left and right are equal.
  [[nodiscard]] BindingPower getPrefixPower(TokenType op);

  /// Get the infix binding power of an operator.
  /// @param op The operator.
  /// @returns The left and right binding power as a pair.
  [[nodiscard]] BindingPower getInfixPower(TokenType op);

  TokenStream& m_token_stream;
  DiagnosticsManager& m_diag_manager;
};
} // namespace chocopy