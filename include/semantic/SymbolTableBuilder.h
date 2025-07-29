#pragma once

#include "SymbolTable.h"
#include "ast/ASTVisitor.h"
#include "ast/VariableDefinition.h"
#include "diagnostics/DiagnosticsManager.h"

namespace chocopy {
class SymbolTableBuilder : public ASTVisitor {
public:
  using ASTVisitor::visit;

  SymbolTableBuilder(DiagnosticsManager& diagnostics_manager)
      : m_diag_manager{diagnostics_manager} {};

  std::any visit(const ast::VariableDefinition& ctx) override;

  SymbolTable& getSymbolTable() { return m_symbol_table; };

private:
  DiagnosticsManager& m_diag_manager;
  SymbolTable m_symbol_table;
};
} // namespace chocopy