#include "ASTVisitor.h"

namespace chocopy {
std::any ASTVisitor::visit(const ProgramContext& ctx) {
  for (const auto& child : ctx.getChildren()) {
    std::visit(
        [this](const auto& child) {
          if (child) {
            visit(*child);
          }
        },
        child);
  }

  return {};
}

std::any ASTVisitor::visit(const VarDefContext& ctx) {
  visit(*ctx.getType());
  visit(*ctx.getValue());

  return {};
}

std::any ASTVisitor::visit(const ExprContext& ctx) {
  return ctx.visit([this](const auto& expr) { return visit(*expr); });
}

std::any ASTVisitor::visit(const ConstantExprContext& ctx) {
  return ctx.visit([this](const auto& cexpr) { return visit(*cexpr); });
}

std::any ASTVisitor::visit(const StmtContext& ctx) {
  return ctx.visit([this](const auto& stmt) { return visit(*stmt); });
}

std::any ASTVisitor::visit(const SimpleStmtContext& ctx) {
  return ctx.visit(
      [this](const auto& simple_stmt) { return visit(*simple_stmt); });
}

std::any ASTVisitor::visit(const AssignmentStmtContext& ctx) {
  return {};
}

std::any ASTVisitor::visit(const TypeContext& ctx) { return {}; }

std::any ASTVisitor::visit(const LiteralContext& ctx) {
  return {};
}
}