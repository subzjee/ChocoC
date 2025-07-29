#include "semantic/TypeEnvironment.h"

namespace chocopy {
[[nodiscard]] OptionalRef<const Type>
TypeEnvironment::typeOf(const ast::Identifier& identifier) {
  const auto entry = m_symbol_table.getEntry(identifier.getValue());

  if (!entry) {
    return std::nullopt;
  }

  if (std::holds_alternative<sema::Variable>(entry.value().get())) {
    return std::get<sema::Variable>(entry.value().get()).type;
  }

  std::unreachable();
}
} // namespace chocopy