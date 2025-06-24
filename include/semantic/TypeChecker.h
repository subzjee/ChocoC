#pragma once

#include "ASTVisitor.h"
#include "DiagnosticsManager.h"
#include "parser/ParseContext.h"
#include "semantic/TypeEnvironment.h"

#include "llvm/Support/FormatVariadic.h"

namespace chocopy {
class TypeChecker : ASTVisitor {
public:
  using ASTVisitor::visit;

  TypeChecker(SymbolTable& symbol_table,
              DiagnosticsManager& diagnostics_manager)
      : m_diag_manager{diagnostics_manager}, local_env(symbol_table) {};

  virtual std::any visit(const VarDefContext& ctx) override;
  virtual std::any visit(const AssignmentStmtContext& ctx) override;

private:
  DiagnosticsManager& m_diag_manager;
  TypeEnvironment local_env;
};
} // namespace chocopy