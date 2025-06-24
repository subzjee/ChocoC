#pragma once

#include "ast/WithLocation.h"
#include "lexer/Token.h"

#include "llvm/Support/SourceMgr.h"

namespace chocopy::ast {
class Literal : public WithLocation {
public:
  Literal(const Token& value) : m_value(value) {};

  /// Get the value of the literal.
  /// @returns The value.
  [[nodiscard]] const TokenValue& getValue() const {
    return m_value.getValue();
  };

  /// Get the type of the literal.
  /// @return The type.
  [[nodiscard]] const TokenType& getType() const { return m_value.getType(); };

  /// Get the location of the literal.
  /// @returns The location.
  [[nodiscard]] llvm::SMRange getLocation() const override {
    return m_value.getLocation();
  };

private:
  const Token& m_value;
};
} // namespace chocopy::ast