#pragma once

#include "parser/ParseContext.h"

#include <any>
#include <memory>

namespace chocopy {
struct ASTVisitor {
  virtual ~ASTVisitor() = default;

  virtual std::any visit(const ProgramContext& ctx);
  virtual std::any visit(const VarDefContext& ctx);
  virtual std::any visit(const ExprContext& ctx);
  virtual std::any visit(const ConstantExprContext& ctx);
  virtual std::any visit(const StmtContext& ctx);
  virtual std::any visit(const SimpleStmtContext& ctx);
  virtual std::any visit(const AssignmentStmtContext& ctx);
  virtual std::any visit(const TypeContext& ctx);
  virtual std::any visit(const LiteralContext& ctx);
};
} // namespace chocopy