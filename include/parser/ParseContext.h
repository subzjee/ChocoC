#pragma once

#include "lexer/Token.h"

#include <memory>
#include <vector>

namespace chocopy {
class LiteralContext {
public:
  LiteralContext(const Token& value) : m_value(value) {};

  [[nodiscard]] const Token& getValue() const { return m_value; };

private:
  const Token& m_value;
};

class TypeContext {
public:
  TypeContext(const Token& base_type, unsigned int dimension = 0)
      : m_base_type(base_type), m_dimension(dimension) {};

  [[nodiscard]] const Token& getBaseType() const { return m_base_type; };
  [[nodiscard]] unsigned int getDimension() const { return m_dimension; };

private:
  const Token& m_base_type;
  const unsigned int m_dimension;
};

class TypedVarContext {
public:
  TypedVarContext(const Token name, std::unique_ptr<TypeContext>& type)
      : m_name(std::move(name)), m_type(std::move(type)) {};

  [[nodiscard]] const Token& getName() const { return m_name; };
  [[nodiscard]] std::unique_ptr<TypeContext>& getType() { return m_type; };

private:
  const Token m_name;
  std::unique_ptr<TypeContext> m_type;
};

class VarDefContext {
public:
  VarDefContext(std::unique_ptr<TypedVarContext>& typed_var,
                std::unique_ptr<LiteralContext>& value)
      : m_name(typed_var->getName()), m_type(std::move(typed_var->getType())),
        m_value(value->getValue()) {};

  [[nodiscard]] const Token& getName() const { return m_name; };
  [[nodiscard]] const std::unique_ptr<TypeContext>& getType() const { return m_type; };
  [[nodiscard]] const Token& getValue() const { return m_value; };

private:
  const Token& m_name;
  const std::unique_ptr<TypeContext> m_type;
  const Token& m_value;
};

using ProgramChildren = std::vector<std::variant<std::unique_ptr<VarDefContext>>>;
class ProgramContext {
public:
  ProgramContext(ProgramChildren children) : m_children(std::move(children)) {};

  [[nodiscard]] const ProgramChildren& getChildren() const { return m_children; };

private:
  const ProgramChildren m_children;
};
} // namespace chocopy