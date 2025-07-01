#include "ast/VariableDefinition.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
  std::any VariableDefinition::accept(ASTVisitor& visitor) const { return visitor.visit(*this); }
}