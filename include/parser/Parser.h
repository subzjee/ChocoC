#pragma once

#include "DiagnosticsManager.h"
#include "lexer/Token.h"
#include "parser/ParseContext.h"

namespace chocopy {
class Parser {
public:
  Parser(std::span<const Token> tokens, DiagnosticsManager& diagnostics_manager)
      : m_tokens(tokens), m_diag_manager(diagnostics_manager) {};

  [[nodiscard]] std::unique_ptr<ProgramContext> parse() {
    return parseProgram();
  };

private:
  friend class ParserTest;

  [[nodiscard]] std::optional<std::reference_wrapper<const Token>>
  peek(const int n = 0) const;

  std::optional<std::reference_wrapper<const Token>> advance();

  template <typename... TokenTypes>
  [[nodiscard]] bool match(const TokenTypes&... token_types) {
    const auto current_token = peek();

    if (!current_token) {
      return false;
    }

    if (((current_token->get().getType() == token_types) || ...)) {
      advance();
      return true;
    }

    return false;
  }

  [[nodiscard]] std::unique_ptr<ProgramContext> parseProgram();
  [[nodiscard]] std::unique_ptr<TypedVarContext> parseTypedVar();
  [[nodiscard]] std::unique_ptr<TypeContext> parseType();
  [[nodiscard]] std::unique_ptr<VarDefContext> parseVarDef();
  [[nodiscard]] std::unique_ptr<LiteralContext> parseLiteral();
  [[nodiscard]] std::unique_ptr<TargetContext> parseTarget();
  [[nodiscard]] std::unique_ptr<ExprContext> parseExpr();
  [[nodiscard]] std::unique_ptr<ConstantExprContext> parseConstantExpr();
  [[nodiscard]] std::unique_ptr<StmtContext> parseStmt();
  [[nodiscard]] std::unique_ptr<SimpleStmtContext> parseSimpleStmt();
  [[nodiscard]] std::unique_ptr<AssignmentStmtContext> parseAssignStmt();

  std::span<const Token> m_tokens;
  std::size_t m_current_idx{0};
  DiagnosticsManager& m_diag_manager;
};
} // namespace chocopy