#pragma once

#include "ast/ASTNode.h"
#include "ast/Statement.h"
#include "ast/VariableDefinition.h"

#include <memory>
#include <variant>
#include <vector>

namespace chocopy {
using ProgramChildren =
    std::vector<std::variant<std::unique_ptr<ast::VariableDefinition>,
                             std::unique_ptr<ast::Statement>>>;
namespace ast {
class Program : public ASTNode {
public:
  Program(ProgramChildren& children) : m_children(std::move(children)) {};

  /// Get all children nodes.
  /// @returns The children nodes.
  [[nodiscard]] const ProgramChildren& getChildren() const {
    return m_children;
  };

  std::any accept(ASTVisitor& visitor) const override;

private:
  const ProgramChildren m_children;
};
} // namespace ast
} // namespace chocopy