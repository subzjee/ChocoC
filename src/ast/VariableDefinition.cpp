#include "ast/VariableDefinition.h"
#include "ASTVisitor.h"

namespace chocopy::ast {
  std::any VariableDefinition::accept(ASTVisitor& visitor) { return visitor.visit(*this); }
}