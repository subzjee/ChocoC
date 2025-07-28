#include "semantic/TypeEnvironment.h"
#include "ast/BinaryExpression.h"
#include "ast/ConstantExpression.h"
#include "ast/Expression.h"
#include "ast/Identifier.h"
#include "ast/Literal.h"
#include "semantic/Variable.h"

#include <utility>
#include <variant>

namespace chocopy {
[[nodiscard]] const Type&
TypeEnvironment::typeOf(const ast::Identifier& identifier) {
  const auto entry = m_symbol_table.getEntry(identifier.getValue());
  assert(entry && "Symbol table does not contain an entry by this name");

  if (std::holds_alternative<sema::Variable>(entry.value().get())) {
    return std::get<sema::Variable>(entry.value().get()).type;
  }
  
  std::unreachable();
}
} // namespace chocopy