#pragma once

#include "ast/AssignmentStatement.h"
#include "ast/BinaryExpression.h"
#include "ast/ConstantExpression.h"
#include "ast/Expression.h"
#include "ast/Program.h"
#include "ast/SimpleStatement.h"
#include "ast/Statement.h"
#include "ast/VariableDefinition.h"

#include <any>

namespace chocopy {
struct ASTVisitor {
  virtual ~ASTVisitor() = default;

  virtual std::any visit(const ast::Program& ctx);
  virtual std::any visit(const ast::VariableDefinition& ctx);
  virtual std::any visit(const ast::AssignmentStatement& ctx);
  virtual std::any visit(const ast::Type& ctx);
  virtual std::any visit(const ast::Literal& ctx);
  virtual std::any visit(const ast::BinaryExpression& ctx);
  virtual std::any visit(const ast::Target& ctx);
};
} // namespace chocopy