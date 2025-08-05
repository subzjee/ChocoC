#pragma once

#include "ast/ASTNode.h"
#include "ast/Statement.h"

namespace chocopy::ast {
class Expression : public Statement {
public:
  virtual ~Expression() = default;
  virtual llvm::SMRange getLocation() const = 0;

  [[nodiscard]] bool isConstantExpression() const { return m_is_cexpr; }
  [[nodiscard]] bool isValidTarget() const { return getKind() == NK_Identifier; }

  /// Check whether \p node is an Expression.
  /// @returns Whether \p node is an Expression.
  static bool classof(const ASTNode* node);

protected:
  Expression(NodeKind kind, bool is_cexpr) : Statement(kind), m_is_cexpr(is_cexpr) {};
  
private:
  bool m_is_cexpr;
};
} // namespace chocopy::ast