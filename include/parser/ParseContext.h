#pragma once

#include "lexer/Token.h"

#include "llvm/ADT/ArrayRef.h"

#include <llvm-21/llvm/ADT/SmallVector.h>
#include <llvm-21/llvm/Support/SMLoc.h>
#include <memory>
#include <vector>

namespace chocopy {
class ParseContext {
public:
  virtual ~ParseContext() = default;

  [[nodiscard]] virtual const llvm::SMRange& getLocation() const = 0;
};

class LiteralContext : ParseContext {
public:
  LiteralContext(const Token& value) : m_value(value) {};

  [[nodiscard]] const Token& getValue() const { return m_value; };
  [[nodiscard]] const llvm::SMRange& getLocation() const { return m_value.getLocation(); };

private:
  const Token& m_value;
};

class TypeContext : ParseContext {
public:
  TypeContext(const Token& base_type, unsigned int dimension = 0)
      : m_base_type(base_type), m_dimension(dimension) {};

  [[nodiscard]] const Token& getBaseType() const { return m_base_type; };
  [[nodiscard]] unsigned int getDimension() const { return m_dimension; };
  [[nodiscard]] std::string getText() const {
    return std::string(m_dimension, '[') +
           std::get<std::string>(m_base_type.getValue()) +
           std::string(m_dimension, ']');
  };
  [[nodiscard]] const llvm::SMRange& getLocation() const { return m_base_type.getLocation(); };

private:
  const Token& m_base_type;
  const unsigned int m_dimension;
};

class TargetContext {
public:
  TargetContext(const Token& name) : m_name(name) {};

  [[nodiscard]] const Token& getName() const { return m_name; }
  [[nodiscard]] const llvm::SMRange& getLocation() const { return m_name.getLocation(); };

private:
  const Token& m_name;
};

using ConstantExpr = std::variant<std::unique_ptr<LiteralContext>>;
class ConstantExprContext : ParseContext {
public:
  ConstantExprContext(ConstantExpr&& cexpr) : m_cexpr(std::move(cexpr)) {};

  [[nodiscard]] const llvm::SMRange& getLocation() const { 
    return std::visit([](const auto& cexpr) -> const llvm::SMRange& {
      return cexpr->getLocation();
    }, m_cexpr);
  };

  template <typename Visitor>
  auto visit(Visitor&& visitor) const {
    return std::visit(std::forward<Visitor>(visitor), m_cexpr);
  }

private:
  const ConstantExpr m_cexpr;
};

using Expr = std::variant<std::unique_ptr<ConstantExprContext>>;
class ExprContext : ParseContext {
public:
  ExprContext(Expr&& expr) : m_expr(std::move(expr)) {};

  [[nodiscard]] const llvm::SMRange& getLocation() const { 
    return std::visit([](const auto& expr) -> const llvm::SMRange& {
      return expr->getLocation();
    }, m_expr);
  };

  template <typename Visitor>
  auto visit(Visitor&& visitor) const {
    return std::visit(std::forward<Visitor>(visitor), m_expr);
  }

private:
  const Expr m_expr;
};

class AssignmentStmtContext {
public:
  AssignmentStmtContext(const std::vector<TargetContext>& targets,
                        std::unique_ptr<ExprContext>& expr)
      : m_targets(targets), m_expr(std::move(expr)) {};

  [[nodiscard]] const llvm::ArrayRef<TargetContext> getTargets() const {
    return m_targets;
  }
  [[nodiscard]] const ExprContext& getExpr() const { return *m_expr; }

private:
  const std::vector<TargetContext> m_targets;
  const std::unique_ptr<ExprContext> m_expr;
};

using SimpleStmtContext = std::variant<std::unique_ptr<AssignmentStmtContext>>;
using StmtContext = std::variant<std::unique_ptr<SimpleStmtContext>>;

class TypedVarContext {
public:
  TypedVarContext(const Token& name, std::unique_ptr<TypeContext>& type)
      : m_name(std::move(name)), m_type(std::move(type)) {};

  [[nodiscard]] const Token& getName() const { return m_name; };
  [[nodiscard]] std::unique_ptr<TypeContext>& getType() { return m_type; };

private:
  const Token& m_name;
  std::unique_ptr<TypeContext> m_type;
};

class VarDefContext {
public:
  VarDefContext(std::unique_ptr<TypedVarContext>& typed_var,
                std::unique_ptr<LiteralContext>& value)
      : m_name(typed_var->getName()), m_type(std::move(typed_var->getType())),
        m_value(std::move(value)) {};

  [[nodiscard]] const Token& getName() const { return m_name; };
  [[nodiscard]] const std::unique_ptr<TypeContext>& getType() const {
    return m_type;
  };
  [[nodiscard]] const std::unique_ptr<LiteralContext>& getValue() const {
    return m_value;
  };

private:
  const Token& m_name;
  const std::unique_ptr<TypeContext> m_type;
  const std::unique_ptr<LiteralContext> m_value;
};

using ProgramChildren = std::vector<
    std::variant<std::unique_ptr<VarDefContext>, std::unique_ptr<StmtContext>>>;
class ProgramContext {
public:
  ProgramContext(ProgramChildren& children)
      : m_children(std::move(children)) {};

  [[nodiscard]] const ProgramChildren& getChildren() const {
    return m_children;
  };

private:
  const ProgramChildren m_children;
};
} // namespace chocopy