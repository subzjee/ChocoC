#pragma once

#include "llvm/Support/Casting.h"

#include <any>

namespace chocopy {
struct ASTVisitor;

namespace ast {
class ASTNode {
public:
  enum NodeKind {
    /* Expressions*/
    NK_Expression,
    NK_BinaryExpression,
    NK_GroupingExpression,
    NK_Identifier,
    NK_Literal,
    NK_UnaryExpression,
    NK_SentinelExpression,

    /* Statements */
    NK_Statement,
    NK_AssignmentStatement,
    NK_SentinelStatement,

    NK_Block,
    NK_Program,
    NK_Type,
    NK_VariableDefinition
  };

  ASTNode(NodeKind kind) : m_kind{kind} {}
  virtual ~ASTNode() = default;

  [[nodiscard]] NodeKind getKind() const { return m_kind; }
  
  virtual std::any accept(ASTVisitor& visitor) const = 0;

private:
  const NodeKind m_kind;
};
} // namespace ast
} // namespace chocopy