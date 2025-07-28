#pragma once

#include "ast/ConstantExpression.h"
#include "ast/Expression.h"
#include "ast/WithLocation.h"
#include "lexer/Token.h"

#include "llvm/ADT/StringRef.h"

#include <memory>
#include <vector>

namespace chocopy::ast {
class CallExpression : public ConstantExpression {
public:
  CallExpression(const Token& callee,
                 std::vector<std::unique_ptr<ast::Expression>> arguments)
      : m_callee{callee}, m_args(std::move(arguments)) {};

  [[nodiscard]] const Token& getCallee() const { return m_callee; }
  [[nodiscard]] std::span<const std::unique_ptr<ast::Expression>>
  getArguments() const {
    return m_args;
  }

  [[nodiscard]] llvm::SMRange getLocation() const override {
    return {m_callee.getLocation().Start, m_args.back()->getLocation().End};
  };

  std::any accept(ASTVisitor& visitor) const override;

private:
  const Token& m_callee;
  std::vector<std::unique_ptr<ast::Expression>> m_args;
};
} // namespace chocopy::ast