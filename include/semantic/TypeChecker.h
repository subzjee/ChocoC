#pragma once

#include "ASTVisitor.h"
#include "DiagnosticsManager.h"
#include "semantic/TypeEnvironment.h"
#include <llvm-21/llvm/Support/FormatVariadic.h>

namespace chocopy {
class TypeChecker : ASTVisitor {
public:
  using ASTVisitor::visit;

  TypeChecker(const SymbolTable& symbol_table,
              DiagnosticsManager& diagnostics_manager)
      : m_diag_manager{diagnostics_manager}, local_env(symbol_table) {};

  virtual void visit(const VarDefContext& ctx) override {
    const Type& lhs = local_env.typeOf(std::get<std::string>(ctx.getName().getValue()));
    const Type& rhs = local_env.typeOf(ctx.getValue()->getValue());

    if (!local_env.isAssignmentCompatible(rhs, lhs)) {
      m_diag_manager.addError(
          llvm::formatv("type mismatch: expected: {0}, got: {1}",
                        lhs.toString(), rhs.toString()),
          ctx.getValue()->getValue().getLocation());
    }
  }

private:
  DiagnosticsManager& m_diag_manager;
  TypeEnvironment local_env;
};
} // namespace chocopy