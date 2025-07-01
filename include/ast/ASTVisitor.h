#pragma once

#include "ast/AssignmentStatement.h"
#include "ast/BinaryExpression.h"
#include "ast/ConstantExpression.h"
#include "ast/Identifier.h"
#include "ast/Program.h"
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
  virtual std::any visit(const ast::Identifier& ctx);
};
} // namespace chocopy