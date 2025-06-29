#include "semantic/TypeChecker.h"
#include "ast/AssignmentStatement.h"
#include "ast/VariableDefinition.h"

#include "llvm/Support/FormatVariadic.h"

namespace chocopy {
std::any TypeChecker::visit(const ast::VariableDefinition& ctx) {
  const Type& lhs =
      local_env.typeOf(std::get<std::string>(ctx.getName().getValue()));
  const Type& rhs = local_env.typeOf(*ctx.getValue());

  if (!local_env.isAssignmentCompatible(rhs, lhs)) {
    m_diag_manager.addError(
        llvm::formatv("type mismatch: expected: {0}, got: {1}", lhs.toString(),
                      rhs.toString()),
        ctx.getValue()->getLocation());
  }

  return {};
}

std::any TypeChecker::visit(const ast::AssignmentStatement& ctx) {
  const auto type_rhs = local_env.typeOf(*ctx.getExpr());

  for (const auto& target : ctx.getTargets()) {
    const auto name = std::get<std::string>(target.getName().getValue());
    const Type& type_lhs = local_env.typeOf(name);

    if (!local_env.isAssignmentCompatible(type_rhs, type_lhs)) {
      m_diag_manager.addError(
          llvm::formatv("type mismatch: expected: {0}, got: {1}",
                        type_lhs.toString(), type_rhs.toString()),
          ctx.getExpr()->getLocation(),
          {target.getLocation(), ctx.getExpr()->getLocation()});
    }
  }

  return {};
}
} // namespace chocopy