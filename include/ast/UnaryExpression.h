#pragma once

#include "ast/ConstantExpression.h"
#include "ast/Expression.h"
#include "lexer/Token.h"

#include <memory>
#include <type_traits>

namespace chocopy::ast {

template <typename ExpressionKind>
  requires std::is_same_v<std::decay_t<ExpressionKind>, ast::Expression> ||
           std::is_same_v<std::decay_t<ExpressionKind>, ast::ConstantExpression>
class UnaryExpression : public ExpressionKind {
public:
  UnaryExpression(const Token& op, std::unique_ptr<ExpressionKind> rhs)
      : m_op(op), m_rhs(std::move(rhs)) {
#ifndef NDEBUG
    if constexpr (std::is_same_v<std::decay_t<ExpressionKind>,
                                 ast::Expression>) {
      assert(op.getType() == TokenType::NOT);
    } else {
      assert(op.getType() == TokenType::MINUS);
    }
#endif
  };

  [[nodiscard]] const Token& getOperator() const { return m_op; }
  [[nodiscard]] const std::unique_ptr<ExpressionKind>& getRHS() const {
    return m_rhs;
  };

  [[nodiscard]] llvm::SMRange getLocation() const override {
    return {m_op.getLocation().Start, m_rhs->getLocation().End};
  }

  std::any accept(ASTVisitor& visitor) const override;

private:
  const Token& m_op;
  std::unique_ptr<ExpressionKind> m_rhs;
};
} // namespace chocopy::ast