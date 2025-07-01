#pragma once

#include "ast/ConstantExpression.h"
#include "lexer/Token.h"

#include <memory>

namespace chocopy::ast {

template <typename ExpressionKind>
class BinaryExpression : public ExpressionKind {
public:
  BinaryExpression(std::unique_ptr<ExpressionKind> lhs, const Token& op,
                     std::unique_ptr<ExpressionKind> rhs)
      : m_lhs(std::move(lhs)), m_op(op), m_rhs(std::move(rhs)) {};

  [[nodiscard]] const std::unique_ptr<ExpressionKind>& getLHS() const { return m_lhs; };
  [[nodiscard]] const Token& getOperator() const { return m_op; }
  [[nodiscard]] const std::unique_ptr<ExpressionKind>& getRHS() const { return m_rhs; };

  [[nodiscard]] llvm::SMRange getLocation() const override {
    return {m_lhs->getLocation().Start, m_rhs->getLocation().End};
  }

  std::any accept(ASTVisitor& visitor) const override;

private:
  std::unique_ptr<ExpressionKind> m_lhs;
  const Token& m_op;
  std::unique_ptr<ExpressionKind> m_rhs;
};
} // namespace chocopy::ast