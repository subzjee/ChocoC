#include "ast/ASTVisitor.h"
#include "ast/AssignmentStatement.h"
#include "ast/BinaryExpression.h"
#include "ast/ConstantExpression.h"
#include "ast/Expression.h"
#include "ast/Identifier.h"
#include "ast/Literal.h"
#include "ast/Program.h"
#include "ast/VariableDefinition.h"

namespace chocopy {
std::any ASTVisitor::visit(const ast::Program& ctx) {
  for (const auto& child : ctx.getChildren()) {
    std::visit(
        [this](const auto& child) {
          if (child) {
            child->accept(*this);
          }
        },
        child);
  }

  return {};
}

std::any ASTVisitor::visit(const ast::VariableDefinition& ctx) {
  ctx.getType()->accept(*this);
  ctx.getValue()->accept(*this);

  return {};
}

std::any ASTVisitor::visit(const ast::AssignmentStatement& ctx) {
  for (auto& target : ctx.getTargets()) {
    target.accept(*this);
  }

  ctx.getExpr()->accept(*this);

  return {};
}

std::any ASTVisitor::visit(const ast::Type& ctx) { return {}; }

std::any ASTVisitor::visit(const ast::Literal& ctx) { return {}; }

std::any ASTVisitor::visit(const ast::BinaryExpression<ast::Expression>& ctx) {
  ctx.getLHS()->accept(*this);
  ctx.getRHS()->accept(*this);

  return {};
}

std::any
ASTVisitor::visit(const ast::BinaryExpression<ast::ConstantExpression>& ctx) {
  ctx.getLHS()->accept(*this);
  ctx.getRHS()->accept(*this);

  return {};
}

std::any ASTVisitor::visit(const ast::UnaryExpression<ast::Expression>& ctx) {
  ctx.getRHS()->accept(*this);

  return {};
}

std::any
ASTVisitor::visit(const ast::UnaryExpression<ast::ConstantExpression>& ctx) {
  ctx.getRHS()->accept(*this);

  return {};
}

std::any ASTVisitor::visit(const ast::Target& ctx) { return {}; }

std::any ASTVisitor::visit(const ast::Identifier& ctx) { return {}; }

std::any ASTVisitor::visit(const ast::Block& ctx) {
  for (const auto& statement : ctx.getStatements()) {
    statement->accept(*this);
  }

  return {};
}

std::any ASTVisitor::visit(const ast::GroupingExpression& ctx) {
  ctx.getExpression()->accept(*this);

  return {};
}
} // namespace chocopy