#pragma once

#include "ast/Statement.h"
#include "ast/VariableDefinition.h"

#include <memory>
#include <variant>

namespace chocopy {
using ProgramChildren =
    std::vector<std::variant<std::unique_ptr<ast::VariableDefinition>,
                             std::unique_ptr<ast::Statement>>>;
namespace ast {
class Program {
public:
  Program(ProgramChildren& children) : m_children(std::move(children)) {};

  /// Get all children nodes.
  /// @returns The children nodes.
  [[nodiscard]] const ProgramChildren& getChildren() const {
    return m_children;
  };

private:
  const ProgramChildren m_children;
};
} // namespace ast
} // namespace chocopy