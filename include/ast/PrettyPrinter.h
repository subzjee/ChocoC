#pragma once

#include "ASTVisitor.h"

namespace chocopy::ast {
class PrettyPrinter : public ASTVisitor {
  using ASTVisitor::visit;

  std::any visit(const Literal& ctx) override;
};
}