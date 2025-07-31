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
      : Expression(/*is_constant_expression*/ true), m_expr(std::move(expression)),
        m_location{left_paren.getLocation().Start,
                   right_paren.getLocation().End} {}

  [[nodiscard]] llvm::SMRange getLocation() const override {
    return m_location;
  }

  [[nodiscard]] const std::unique_ptr<ast::Expression>& getExpression() const {
    return m_expr;
  }

  std::any accept(ASTVisitor& visitor) const override;

private:
  std::unique_ptr<ast::Expression> m_expr;
  llvm::SMRange m_location;
};
} // namespace chocopy::ast