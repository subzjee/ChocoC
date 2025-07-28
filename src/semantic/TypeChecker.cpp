#include "semantic/TypeChecker.h"
#include "ast/AssignmentStatement.h"
#include "ast/GroupingExpression.h"
#include "ast/Identifier.h"
#include "ast/VariableDefinition.h"

#include "llvm/Support/FormatVariadic.h"

namespace chocopy {
std::any TypeChecker::visit(const ast::VariableDefinition& ctx) {
  const auto lhs_type = m_local_env.typeOf(*ctx.getName());
  const auto rhs_type = std::any_cast<std::optional<const Type>>(ctx.getValue()->accept(*this));

  if (!rhs_type) {
    return {};
  }

  if (!rhs_type->isAssignmentCompatible(lhs_type)) {
    m_diag_manager.addError(
        llvm::formatv("type mismatch: expected: {0}, got: {1}", lhs_type.toString(),
                      rhs_type->toString()),
        ctx.getValue()->getLocation());
  }

  return {};
}

std::any TypeChecker::visit(const ast::AssignmentStatement& ctx) {
  const auto rhs_type = std::any_cast<std::optional<const Type>>(ctx.getExpr()->accept(*this));

  if (!rhs_type) {
    return {};
  }

  for (const auto& target : ctx.getTargets()) {
    const auto lhs_type = std::any_cast<std::optional<const Type>>(target->accept(*this));

    if (!lhs_type) {
      continue;
    }

    if (!rhs_type->isAssignmentCompatible(*lhs_type)) {
      m_diag_manager.addError(
          llvm::formatv("type mismatch: expected: {0}, got: {1}",
                        lhs_type->toString(), rhs_type->toString()),
          ctx.getExpr()->getLocation(),
          {target->getLocation(), ctx.getExpr()->getLocation()});
    }
  }

  return {};
}

std::any TypeChecker::visit(const ast::Identifier& ctx) {
  return std::optional<const Type>(m_local_env.typeOf(ctx));
}

std::any TypeChecker::visit(const ast::Literal& ctx) {
  const auto type = ctx.getType();

  if (type == "bool") {
    return std::optional<const Type>(*Type::getBooleanType());
  } else if (type == "str") {
    return std::optional<const Type>(*Type::getStringType());
  } else if (type == "int") {
    return std::optional<const Type>(*Type::getIntegerType());
  } else if (type == "<None>") {
    return std::optional<const Type>(*Type::getNoneType());
  }

  return std::optional<const Type>{};
}

std::any TypeChecker::visit(const ast::BinaryExpression<ast::Expression>& ctx) {
  const auto lhs_type = std::any_cast<std::optional<const Type>>(ctx.getLHS()->accept(*this));
  const auto rhs_type = std::any_cast<std::optional<const Type>>(ctx.getRHS()->accept(*this));

  if (!lhs_type || !rhs_type) {
    return std::optional<const Type>{};
  }

  switch (ctx.getOperator().getType()) {
    case TokenType::AND:
      [[fallthrough]];
    case TokenType::OR:
      if (!lhs_type->isBoolean() || !rhs_type->isBoolean()) {
        m_diag_manager.addError(
          llvm::formatv("unsupported operand type(s) for '{0}': '{1}' and '{2}'",
                        ctx.getOperator().getText(), lhs_type->toString(), rhs_type->toString()),
          ctx.getLocation());
          return std::optional<const Type>{};
      }
      
      return std::optional<const Type>(*Type::getBooleanType());
    default:
      return std::optional<const Type>{};
  }
}

std::any TypeChecker::visit(const ast::BinaryExpression<ast::ConstantExpression>& ctx) {
  const auto lhs_type = std::any_cast<std::optional<const Type>>(ctx.getLHS()->accept(*this));
  const auto rhs_type = std::any_cast<std::optional<const Type>>(ctx.getRHS()->accept(*this));

  if (!lhs_type || !rhs_type) {
    return std::optional<const Type>{};
  }

  return std::optional<const Type>{};
}

std::any TypeChecker::visit(const ast::GroupingExpression& ctx) {
  return ctx.getExpression()->accept(*this);
}
} // namespace chocopy