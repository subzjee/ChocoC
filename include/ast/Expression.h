#pragma once

#include "ast/Statement.h"

namespace chocopy::ast {
class Expression : public Statement {
public:
  enum ExpressionKind {
    EK_BinaryExpression,
    EK_GroupingExpression,
    EK_Identifier,
    EK_Literal,
    EK_UnaryExpression
  };

  virtual ~Expression() = default;
  virtual llvm::SMRange getLocation() const = 0;

  [[nodiscard]] bool isConstantExpression() const { return m_is_constant_expression; }
  [[nodiscard]] ExpressionKind getKind() const { return m_kind; }

  /// Check whether \p statement is an Expression.
  /// @returns Whether \p statement is an Expression.
  static bool classof(const Statement* statement) { return statement->getKind() == StatementKind::SK_Expression; }

protected:
  Expression(ExpressionKind kind, bool is_constant_expression) : Statement(Statement::StatementKind::SK_Expression), m_kind{kind}, m_is_constant_expression(is_constant_expression) {};
  
private:
  const ExpressionKind m_kind;
  bool m_is_constant_expression;
};
} // namespace chocopy::ast