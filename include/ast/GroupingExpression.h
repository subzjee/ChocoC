#pragma once

#include "ast/Expression.h"
#include "lexer/Token.h"

#include <memory>

namespace chocopy::ast {
class GroupingExpression : public Expression {
public:
  GroupingExpression(const Token& left_paren,
                     std::unique_ptr<ast::Expression> expression,
                     const Token& right_paren)
      : Expression(ExpressionKind::EK_GroupingExpression, /*is_cexpr*/ true), m_expr(std::move(expression)),
        m_location{left_paren.getLocation().Start,
                   right_paren.getLocation().End} {}

  /// Get the source location.
  /// @returns The source location.
  [[nodiscard]] llvm::SMRange getLocation() const override {
    return m_location;
  }

  /// Get the expression within the grouping.
  /// @returns The expression within the grouping.
  [[nodiscard]] const std::unique_ptr<ast::Expression>& getExpression() const {
    return m_expr;
  }

  std::any accept(ASTVisitor& visitor) const override;

  /// Check whether this class is an expression for LLVM's RTTI.
  /// @returns Whether this class is an expression.
  static bool classof(const Expression* expr ) {
    return expr->getKind() == ExpressionKind::EK_GroupingExpression;
  }

private:
  std::unique_ptr<ast::Expression> m_expr;
  llvm::SMRange m_location;
};
} // namespace chocopy::ast