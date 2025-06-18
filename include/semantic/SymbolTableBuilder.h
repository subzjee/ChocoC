#pragma once

#include "ASTVisitor.h"
#include "DiagnosticsManager.h"
#include "semantic/SymbolTable.h"

#include "llvm/Support/FormatVariadic.h"

namespace chocopy {
class SymbolTableBuilder : ASTVisitor {
public:
  using ASTVisitor::visit;

  SymbolTableBuilder(DiagnosticsManager& diagnostics_manager)
      : m_diag_manager{diagnostics_manager} {};

  virtual void visit(const VarDefContext& ctx) override {
    const auto name = std::get<std::string>(ctx.getName().getValue());

    // Check for redefinition within the same table.
    if (m_symbol_table.getEntry(name)) {
      m_diag_manager.addError(llvm::formatv("redefinition of `{0}`", name),
                              ctx.getName().getLocation());
      return;
    }

    const auto type_entry = m_symbol_table.getEntry(ctx.getType()->getText());

    if (!type_entry || !std::holds_alternative<Type>(*type_entry)) {
      m_diag_manager.addError("undefined type",
                              ctx.getType()->getBaseType().getLocation());
      return;
    }

    const auto entry = Variable(name, std::get<Type>(*type_entry));
    m_symbol_table.addEntry(name, entry);
  }

  const SymbolTable& getSymbolTable() const { return m_symbol_table; };

private:
  DiagnosticsManager& m_diag_manager;
  SymbolTable m_symbol_table;
};
} // namespace chocopy