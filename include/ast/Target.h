#pragma once

#include "ast/ASTNode.h"
#include "ast/WithLocation.h"
#include "lexer/Token.h"

namespace chocopy::ast {
class Target : public ASTNode, public WithLocation {
public:
  Target(const Token &name) : m_name(name) {};

  /// Get the name.
  /// @returns The name.
  [[nodiscard]] const Token &getName() const { return m_name; }

  /// Get the location.
  /// @returns The location.
  [[nodiscard]] llvm::SMRange getLocation() const override {
    return m_name.getLocation();
  };

  std::any accept(ASTVisitor &visitor) const override;

private:
  const Token &m_name;
};
} // namespace chocopy::ast