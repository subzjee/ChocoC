#pragma once

#include "ast/ASTNode.h"
#include "ast/WithLocation.h"

namespace chocopy::ast {
class Statement : public ASTNode, public WithLocation {
public:
  virtual ~Statement() = default;

  /// Check whether \p node is a Statement.
  /// @returns Whether \p node is a Statement.
  static bool classof(const ASTNode* node);

protected:
  Statement(NodeKind kind) : ASTNode(kind) {};
};
} // namespace chocopy::ast