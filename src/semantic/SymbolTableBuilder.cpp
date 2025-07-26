#include "semantic/SymbolTableBuilder.h"
#include "ast/AssignmentStatement.h"
#include "ast/VariableDefinition.h"

#include "llvm/Support/FormatVariadic.h"

namespace chocopy {
std::any SymbolTableBuilder::visit(const ast::VariableDefinition& ctx) {
  const auto name = std::get<std::string>(ctx.getName().getValue());

  // Check for redefinition within the same table.
  if (m_symbol_table.getEntry(name)) {
    m_diag_manager.addError(llvm::formatv("redefinition of `{0}`", name),
                            ctx.getName().getLocation());
    return {};
  }

  const auto type_entry = m_symbol_table.getEntry(ctx.getType()->getText());

  if (!type_entry || !std::holds_alternative<Type>(type_entry->get())) {
    m_diag_manager.addError("undefined type",
                            ctx.getType()->getBaseType().getLocation());
    return {};
  }

  const auto entry = Variable(name, std::get<Type>(type_entry->get()));
  m_symbol_table.addEntry(name, entry);

  return {};
}

std::any SymbolTableBuilder::visit(const ast::AssignmentStatement& ctx) {
  for (const auto& target : ctx.getTargets()) {
    const auto name = std::get<std::string>(target->getName().getValue());

    if (!m_symbol_table.getEntry(name)) {
      m_diag_manager.addError(llvm::formatv("undefined variable `{0}`", name),
                              target->getLocation());
    }
  }

  return {};
}
} // namespace chocopy