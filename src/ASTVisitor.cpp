#include "ASTVisitor.h"
#include "ast/AssignmentStatement.h"
#include "ast/ConstantExpression.h"
#include "ast/Expression.h"
#include "ast/Literal.h"
#include "ast/Program.h"
#include "ast/SimpleStatement.h"
#include "ast/Statement.h"
#include "ast/VariableDefinition.h"

namespace chocopy {
std::any ASTVisitor::visit(const ast::Program& ctx) {
  for (const auto& child : ctx.getChildren()) {
    std::visit(
        [this](const auto& child) {
          if (child) {
            visit(*child);
          }
        },
        child);
  }

  return {};
}

std::any ASTVisitor::visit(const ast::VariableDefinition& ctx) {
  visit(*ctx.getType());
  visit(*ctx.getValue());

  return {};
}

std::any ASTVisitor::visit(const ast::Expression& ctx) {
  return ctx.visit([this](const auto& expr) { return visit(*expr); });
}

std::any ASTVisitor::visit(const ast::ConstantExpression& ctx) {
  return ctx.visit([this](const auto& cexpr) { return visit(*cexpr); });
}

std::any ASTVisitor::visit(const ast::Statement& ctx) {
  return ctx.visit([this](const auto& stmt) { return visit(*stmt); });
}

std::any ASTVisitor::visit(const ast::SimpleStatement& ctx) {
  return ctx.visit(
      [this](const auto& simple_stmt) { return visit(*simple_stmt); });
}

std::any ASTVisitor::visit(const ast::AssignmentStatement& ctx) {
  return {};
}

std::any ASTVisitor::visit(const ast::Type& ctx) { return {}; }

std::any ASTVisitor::visit(const ast::Literal& ctx) {
  return {};
}
}