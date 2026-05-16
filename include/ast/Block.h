#pragma once

#include <memory>
#include <vector>

#include "ast/ASTNode.h"
#include "ast/Statement.h"

namespace chocopy::ast {
class Block : public ASTNode {
public:
  Block(std::vector<std::unique_ptr<Statement>> statements)
      : ASTNode(NK_Block), m_statements(std::move(statements)) {};

  [[nodiscard]] const std::vector<std::unique_ptr<Statement>>&
  getStatements() const {
    return m_statements;
  }

  std::any accept(ASTVisitor& visitor) const;

  /// Check whether \p node is a Block.
  /// @returns Whether \p node is a Block.
  static bool classof(const ASTNode* node);

private:
  const std::vector<std::unique_ptr<Statement>> m_statements;
};
} // namespace chocopy::ast
