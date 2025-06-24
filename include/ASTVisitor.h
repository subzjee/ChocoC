#pragma once

#include "ast/AssignmentStatement.h"
#include "ast/BinaryOpExpression.h"
#include "ast/ConstantExpression.h"
#include "ast/Expression.h"
#include "ast/Program.h"
#include "ast/Statement.h"
#include "ast/SimpleStatement.h"
#include "ast/VariableDefinition.h"

#include <any>

namespace chocopy {
struct ASTVisitor {
  virtual ~ASTVisitor() = default;

  virtual std::any visit(const ast::Program& ctx);
  virtual std::any visit(const ast::VariableDefinition& ctx);
  virtual std::any visit(const ast::Expression& ctx);
  virtual std::any visit(const ast::ConstantExpression& ctx);
  virtual std::any visit(const ast::Statement& ctx);
  virtual std::any visit(const ast::SimpleStatement& ctx);
  virtual std::any visit(const ast::AssignmentStatement& ctx);
  virtual std::any visit(const ast::Type& ctx);
  virtual std::any visit(const ast::Literal& ctx);
  virtual std::any visit(const ast::BinaryOpExpression& ctx);
};
} // namespace chocopy