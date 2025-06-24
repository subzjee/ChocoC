#pragma once

#include "lexer/Token.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Support/SMLoc.h"

#include <memory>
#include <variant>
#include <vector>

namespace chocopy {

/// Abstract base class for nodes that have a location.
class WithLocation {
public:
  virtual ~WithLocation() = default;

  [[nodiscard]] virtual llvm::SMRange getLocation() const = 0;
};

class LiteralContext : public WithLocation {
public:
  LiteralContext(const Token& value) : m_value(value) {};

  /// Get the value of the literal.
  /// @returns The value.
  [[nodiscard]] const TokenValue& getValue() const { return m_value.getValue(); };

  /// Get the type of the literal.
  /// @return The type.
  [[nodiscard]] const TokenType& getType() const { return m_value.getType(); };

  /// Get the location of the literal.
  /// @returns The location.
  [[nodiscard]] llvm::SMRange getLocation() const override { return m_value.getLocation(); };

private:
  const Token& m_value;
};

class TypeContext : public WithLocation {
public:
  TypeContext(const Token& base_type, unsigned int dimension = 0)
      : m_base_type(base_type), m_dimension(dimension) {};

  /// Get the base type.
  /// @returns The base type.
  [[nodiscard]] const Token& getBaseType() const { return m_base_type; };

  /// Get the dimension of the type.
  /// @returns The dimension.
  [[nodiscard]] unsigned int getDimension() const { return m_dimension; };

  /// Get the string representation of the type.
  /// @returns The string representation.
  [[nodiscard]] std::string getText() const {
    return std::string(m_dimension, '[') +
           std::get<std::string>(m_base_type.getValue()) +
           std::string(m_dimension, ']');
  };
  [[nodiscard]] llvm::SMRange getLocation() const override { return m_base_type.getLocation(); };

private:
  const Token& m_base_type;
  const unsigned int m_dimension;
};

class TargetContext : public WithLocation {
public:
  TargetContext(const Token& name) : m_name(name) {};

  /// Get the name.
  /// @returns The name.
  [[nodiscard]] const Token& getName() const { return m_name; }

  /// Get the location.
  /// @returns The location.
  [[nodiscard]] llvm::SMRange getLocation() const override { return m_name.getLocation(); };

private:
  const Token& m_name;
};

using ConstantExpr = std::variant<std::unique_ptr<LiteralContext>>;
class ConstantExprContext : public WithLocation {
public:
  ConstantExprContext(ConstantExpr&& cexpr) : m_cexpr(std::move(cexpr)) {};

  /// Get the location.
  /// @returns The location.
  [[nodiscard]] llvm::SMRange getLocation() const override { 
    return std::visit([](const auto& cexpr) -> const llvm::SMRange {
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
class ExprContext : public WithLocation {
public:
  ExprContext(Expr&& expr) : m_expr(std::move(expr)) {};

  /// Get the location.
  /// @returns The location.
  [[nodiscard]] llvm::SMRange getLocation() const override { 
    return std::visit([](const auto& expr) -> const llvm::SMRange {
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

  /// Get all targets to be assigned to.
  /// @returns The targets.
  [[nodiscard]] const llvm::ArrayRef<TargetContext> getTargets() const {
    return m_targets;
  }

  /// Get the expression on the right-hand side.
  /// @returns The expression.
  [[nodiscard]] const std::unique_ptr<ExprContext>& getExpr() const { return m_expr; }

private:
  const std::vector<TargetContext> m_targets;
  const std::unique_ptr<ExprContext> m_expr;
};

using SimpleStmt = std::variant<std::unique_ptr<AssignmentStmtContext>>;
class SimpleStmtContext {
public:
  SimpleStmtContext(SimpleStmt&& statement) : m_stmt(std::move(statement)) {};

  template <typename Visitor>
  auto visit(Visitor&& visitor) const {
    return std::visit(std::forward<Visitor>(visitor), m_stmt);
  }

private:
  const SimpleStmt m_stmt;
};

using Stmt = std::variant<std::unique_ptr<SimpleStmtContext>>;
class StmtContext {
public:
  StmtContext(Stmt&& statement) : m_stmt(std::move(statement)) {};

  template <typename Visitor>
  auto visit(Visitor&& visitor) const {
    return std::visit(std::forward<Visitor>(visitor), m_stmt);
  }

private:
  const Stmt m_stmt;
};

class TypedVarContext {
public:
  TypedVarContext(const Token& name, std::unique_ptr<TypeContext>& type)
      : m_name(name), m_type(std::move(type)) {};

  /// Get the name.
  /// @returns The name.
  [[nodiscard]] const Token& getName() const { return m_name; };

  /// Get the type.
  /// @returns The type.
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

  /// Get the name of the variable.
  /// @returns The name.
  [[nodiscard]] const Token& getName() const { return m_name; };

  /// Get the type of the variable.
  /// @returns The type.
  [[nodiscard]] const std::unique_ptr<TypeContext>& getType() const {
    return m_type;
  };

  /// Get the value of the variable.
  /// @returns The value.
  [[nodiscard]] const std::unique_ptr<LiteralContext>& getValue() const {
    return m_value;
  };

private:
  const Token& m_name;
  std::unique_ptr<TypeContext> m_type;
  std::unique_ptr<LiteralContext> m_value;
};

using ProgramChildren = std::vector<
    std::variant<std::unique_ptr<VarDefContext>, std::unique_ptr<StmtContext>>>;
class ProgramContext {
public:
  ProgramContext(ProgramChildren& children)
      : m_children(std::move(children)) {};

  /// Get all children nodes.
  /// @returns The children nodes.
  [[nodiscard]] const ProgramChildren& getChildren() const {
    return m_children;
  };

private:
  const ProgramChildren m_children;
};
} // namespace chocopy