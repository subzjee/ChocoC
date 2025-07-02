#include "ast/Block.h"
#include "ast/ASTVisitor.h"

namespace chocopy::ast {
std::any Block::accept(ASTVisitor& visitor) const {
  return visitor.visit(*this);
}
} // namespace chocopy::ast