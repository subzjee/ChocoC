#pragma once

#include "ast/ASTNode.h"
#include "ast/WithLocation.h"

namespace chocopy::ast {
class Target : public ASTNode, public WithLocation {
public:
  enum TargetKind { TK_Identifier };

  virtual ~Target() = default;

  [[nodiscard]] TargetKind getKind() const { return m_kind; }

protected:
  Target(TargetKind kind) : m_kind{kind} {};

private:
  const TargetKind m_kind;
};
} // namespace chocopy::ast