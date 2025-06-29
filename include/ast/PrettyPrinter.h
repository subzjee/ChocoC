#pragma once

#include "ASTVisitor.h"
#include "ast/VariableDefinition.h"

#include <sstream>

namespace chocopy::ast {
class PrettyPrinter : public ASTVisitor {
  using ASTVisitor::visit;
public:

  std::any visit(const Program& ctx) override;
  std::any visit(const Literal& ctx) override;
  std::any visit(const VariableDefinition& ctx) override;

private:
  std::ostringstream out;
};
}