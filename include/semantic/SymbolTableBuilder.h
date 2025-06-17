#pragma once

#include "ASTVisitor.h"
#include "DiagnosticsManager.h"
#include "semantic/SymbolTable.h"

namespace chocopy {
class SymbolTableBuilder : ASTVisitor {
public:
  using ASTVisitor::visit;

  SymbolTableBuilder(DiagnosticsManager& diagnostics_manager)
      : m_diag_manager{diagnostics_manager} {};

  virtual void visit(const VarDefContext& ctx) override {
    const auto type = m_symbol_table.getEntry(ctx.getType()->getText());

    if (!type || !std::holds_alternative<Type>(*type)) {
      m_diag_manager.addError("undefined type",
                              ctx.getType()->getBaseType().getLocation());
      return;
    }

    const auto name = ctx.getName().getText();
    const auto entry = Variable(name, std::get<Type>(*type));
    m_symbol_table.addEntry(name, entry);
  }

  const SymbolTable& getSymbolTable() const { return m_symbol_table; };

private:
  DiagnosticsManager& m_diag_manager;
  SymbolTable m_symbol_table;
};
} // namespace chocopy