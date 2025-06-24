#pragma once

#include "ASTVisitor.h"
#include "DiagnosticsManager.h"
#include "SymbolTable.h"

#include "llvm/Support/FormatVariadic.h"

namespace chocopy {
class SymbolTableBuilder : ASTVisitor {
public:
  using ASTVisitor::visit;

  SymbolTableBuilder(DiagnosticsManager& diagnostics_manager)
      : m_diag_manager{diagnostics_manager} {};

  virtual std::any visit(const VarDefContext& ctx) override;

  SymbolTable& getSymbolTable() { return m_symbol_table; };

private:
  DiagnosticsManager& m_diag_manager;
  SymbolTable m_symbol_table;
};
} // namespace chocopy