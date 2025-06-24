#pragma once

#include "ast/AssignmentStatement.h"

#include <memory>
#include <variant>

namespace chocopy::ast {
class SimpleStatement {
  using SimpleStmt = std::variant<std::unique_ptr<AssignmentStatement>>;

public:
  SimpleStatement(SimpleStmt&& statement) : m_stmt(std::move(statement)) {};

  template <typename Visitor> auto visit(Visitor&& visitor) const {
    return std::visit(std::forward<Visitor>(visitor), m_stmt);
  }

private:
  const SimpleStmt m_stmt;
};
} // namespace chocopy::ast