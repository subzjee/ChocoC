#pragma once

#include "ast/SimpleStatement.h"

#include <memory>

namespace chocopy::ast {
class Statement {
  using Stmt = std::variant<std::unique_ptr<SimpleStatement>>;

public:
  Statement(Stmt&& statement) : m_stmt(std::move(statement)) {};

  template <typename Visitor> auto visit(Visitor&& visitor) const {
    return std::visit(std::forward<Visitor>(visitor), m_stmt);
  }

private:
  const Stmt m_stmt;
};
} // namespace chocopy::ast