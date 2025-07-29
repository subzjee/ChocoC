#include "semantic/SymbolTableBuilder.h"

namespace chocopy {
std::any SymbolTableBuilder::visit(const ast::VariableDefinition& ctx) {
  const auto name = ctx.getName()->getValue();

  // Check for redefinition within the same table.
  if (m_symbol_table.getEntry(name)) {
    m_diag_manager.report(DiagID::Redefinition, ctx.getName()->getLocation(),
                          {name.str()});
    return {};
  }

  const auto type_entry = m_symbol_table.getEntry(ctx.getType()->getText());

  if (!type_entry || !std::holds_alternative<Type>(type_entry->get())) {
    m_diag_manager.report(DiagID::UndefinedType,
                          ctx.getType()->getBaseType().getLocation());
    return {};
  }

  const auto entry = Variable(std::get<Type>(type_entry->get()));
  m_symbol_table.addEntry(name, entry);

  return {};
}
} // namespace chocopy