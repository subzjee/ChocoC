#pragma once

#include "ast/ASTNode.h"
#include "ast/WithLocation.h"

namespace chocopy::ast {
class Statement : public ASTNode, public WithLocation {
public:
  enum StatementKind {
    SK_Expression,
    SK_Assignment
  };

  virtual ~Statement() = default;

  [[nodiscard]] StatementKind getKind() const { return m_kind; }

protected:
  Statement(StatementKind kind) : m_kind{kind} {};
  
private:
  const StatementKind m_kind;
};
} // namespace chocopy::ast