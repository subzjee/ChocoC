#pragma once

#include "parser/ParseContext.h"

#include <memory>

namespace chocopy {
struct ASTVisitor {
  virtual ~ASTVisitor() = default;

  virtual void visit(const ProgramContext& ctx) {
    for (const auto& child : ctx.getChildren()) {
      std::visit([this](const auto& child) {
        if (child) {
          visit(*child);
        }
      }, child);
    }
  }
  
  virtual void visit(const VarDefContext& ctx) {
    visit(*ctx.getType());
    visit(*ctx.getValue());
  }

  virtual void visit(const TypeContext& ctx [[maybe_unused]]) { }
  virtual void visit(const LiteralContext& ctx [[maybe_unused]]) { }

};
} // namespace chocopy