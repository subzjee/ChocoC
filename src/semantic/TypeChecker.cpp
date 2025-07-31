#include "semantic/TypeChecker.h"

namespace chocopy {
std::any TypeChecker::visit(const ast::VariableDefinition& ctx) {
  const auto lhs_type = m_local_env.typeOf(*ctx.getName());
  assert(lhs_type && "Variable does not exist in symbol table");

  const auto rhs_type =
      std::any_cast<std::optional<const Type>>(ctx.getValue()->accept(*this));

  if (!rhs_type) {
    return {};
  }

  if (!rhs_type->isAssignmentCompatible(*lhs_type)) {
    m_diag_manager.report(DiagID::TypeMismatch, ctx.getValue()->getLocation(),
                          {lhs_type->get().toString(), rhs_type->toString()});
  }

  return {};
}

std::any TypeChecker::visit(const ast::AssignmentStatement& ctx) {
  const auto rhs_type =
      std::any_cast<std::optional<const Type>>(ctx.getExpr()->accept(*this));

  for (const auto& target : ctx.getTargets()) {
    const auto lhs_type =
        std::any_cast<std::optional<const Type>>(target->accept(*this));

    if (!lhs_type) {
      continue;
    }

    if (rhs_type && !rhs_type->isAssignmentCompatible(*lhs_type)) {
      m_diag_manager.report(
          DiagID::TypeMismatch, ctx.getExpr()->getLocation(),
          {lhs_type->toString(), rhs_type->toString()},
          {target->getLocation(), ctx.getExpr()->getLocation()});
    }
  }

  return {};
}

std::any TypeChecker::visit(const ast::Identifier& ctx) {
  const auto type = m_local_env.typeOf(ctx);

  if (!type) {
    m_diag_manager.report(DiagID::UndefinedName, ctx.getLocation(),
                          {ctx.getValue().str()});
    return std::optional<const Type>{};
  }

  return std::optional<const Type>(*type);
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

std::any TypeChecker::visit(const ast::BinaryExpression& ctx) {
  const auto lhs_type =
      std::any_cast<std::optional<const Type>>(ctx.getLHS()->accept(*this));
  const auto rhs_type =
      std::any_cast<std::optional<const Type>>(ctx.getRHS()->accept(*this));

  if (!lhs_type || !rhs_type) {
    return std::optional<const Type>{};
  }

  switch (ctx.getOperator().getType()) {
  case TokenType::AND:
    [[fallthrough]];
  case TokenType::OR:
    if (!lhs_type->isBoolean() || !rhs_type->isBoolean()) {
      m_diag_manager.report(DiagID::UnsupportedBinaryOperandType,
                            ctx.getLocation(),
                            {ctx.getOperator().getText().str(),
                             lhs_type->toString(), rhs_type->toString()});
      return std::optional<const Type>{};
    }

    return std::optional<const Type>(*Type::getBooleanType());
  default:
    return std::optional<const Type>{};
  }
}

std::any TypeChecker::visit(const ast::GroupingExpression& ctx) {
  return ctx.getExpression()->accept(*this);
}
} // namespace chocopy