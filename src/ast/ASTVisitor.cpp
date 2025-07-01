#include "ast/ASTVisitor.h"
#include "ast/AssignmentStatement.h"
#include "ast/BinaryExpression.h"
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
  for(auto& target : ctx.getTargets()) {
    target.accept(*this);
  }
  
  ctx.getExpr()->accept(*this);

  return {};
}

std::any ASTVisitor::visit(const ast::Type& ctx) { return {}; }

std::any ASTVisitor::visit(const ast::Literal& ctx) { return {}; }

std::any ASTVisitor::visit(const ast::BinaryExpression& ctx) {
  ctx.getLHS()->accept(*this);
  ctx.getRHS()->accept(*this);

  return {};
}

std::any ASTVisitor::visit(const ast::Target& ctx) { return {}; }

std::any ASTVisitor::visit(const ast::Identifier& ctx) { return {}; }
} // namespace chocopy