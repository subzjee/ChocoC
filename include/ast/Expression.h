#pragma once

#include "ast/Statement.h"

namespace chocopy::ast {
class Expression : public Statement {
public:
  virtual ~Expression() = default;
  virtual llvm::SMRange getLocation() const = 0;

  [[nodiscard]] bool isConstantExpression() const { return m_is_constant_expression; }

protected:
  Expression(bool is_constant_expression) : m_is_constant_expression(is_constant_expression) {};
  
private:
  bool m_is_constant_expression;
};
} // namespace chocopy::ast