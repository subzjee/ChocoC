#pragma once

#include "ast/Statement.h"

#include <memory>
#include <vector>

namespace chocopy::ast {
class Block : public ASTNode {
public:
  Block(std::vector<std::unique_ptr<Statement>> statements)
      : m_statements(std::move(statements)) {};

  [[nodiscard]] const std::vector<std::unique_ptr<Statement>>&
  getStatements() const {
    return m_statements;
  }

  std::any accept(ASTVisitor& visitor) const;

private:
  const std::vector<std::unique_ptr<Statement>> m_statements;
};
} // namespace chocopy::ast