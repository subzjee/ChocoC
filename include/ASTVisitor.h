#pragma once

#include "parser/ParseContext.h"

#include <any>
#include <memory>

namespace chocopy {
struct ASTVisitor {
  virtual ~ASTVisitor() = default;

  virtual std::any visit(const ProgramContext& ctx) {
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

  virtual std::any visit(const VarDefContext& ctx) {
    visit(*ctx.getType());
    visit(*ctx.getValue());

    return {};
  }

  virtual std::any visit(const ExprContext& ctx) {
    return ctx.visit([this](const auto& expr) { return visit(*expr); });
  }

  virtual std::any visit(const ConstantExprContext& ctx) {
    return ctx.visit([this](const auto& cexpr) { return visit(*cexpr); });
  }

  virtual std::any visit(const StmtContext& ctx) {
    return std::visit([this](const auto& stmt) { return visit(*stmt); }, ctx);
  }

  virtual std::any visit(const SimpleStmtContext& ctx) {
    return std::visit(
        [this](const auto& simple_stmt) { return visit(*simple_stmt); }, ctx);
  }

  virtual std::any visit(const AssignmentStmtContext& ctx [[maybe_unused]]) { return {}; }

  virtual std::any visit(const TypeContext& ctx [[maybe_unused]]) { return {}; }
  
  virtual std::any visit(const LiteralContext& ctx [[maybe_unused]]) {
    return {};
  }
};
} // namespace chocopy