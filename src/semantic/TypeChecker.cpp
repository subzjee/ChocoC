#include "semantic/TypeChecker.h"

namespace chocopy {
std::any TypeChecker::visit(const VarDefContext& ctx) {
  const Type& lhs =
      local_env.typeOf(std::get<std::string>(ctx.getName().getValue()));
  const Type& rhs = local_env.typeOf(*ctx.getValue());

  if (!local_env.isAssignmentCompatible(rhs, lhs)) {
    m_diag_manager.addError(
        llvm::formatv("type mismatch: expected: {0}, got: {1}",
                      lhs.toString(), rhs.toString()),
        ctx.getValue()->getLocation());
  }

  return {};
}

std::any TypeChecker::visit(const AssignmentStmtContext& ctx) {
  const auto rhs = local_env.typeOf(*ctx.getExpr());

  for (const auto& target : ctx.getTargets()) {
    const Type& lhs =
        local_env.typeOf(std::get<std::string>(target.getName().getValue()));

    if (!local_env.isAssignmentCompatible(rhs, lhs)) {
      m_diag_manager.addError(
          llvm::formatv("type mismatch: expected: {0}, got: {1}",
                        lhs.toString(), rhs.toString()),
          ctx.getExpr()->getLocation(),
          {target.getLocation(), ctx.getExpr()->getLocation()});
    }
  }

  return {};
}
} // namespace chocopy