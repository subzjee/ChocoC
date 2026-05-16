#include "diagnostics/DiagInfo.h"

#include <unordered_map>

#include "llvm/Support/SourceMgr.h"

namespace {
    using llvm::SourceMgr::DK_Error;

    const std::unordered_map<DiagID, DiagInfo> table = {
        // Lexer
        {DiagID::UnexpectedCharacter, {DK_Error, "unexpected character"}},
        {DiagID::LeadingZeros,
         {DK_Error, "an integer may not have leading zeros"}},
        {DiagID::IntegerOutOfRange,
         {DK_Error,
          "an integer must be within the range [-2147483648, 2147483647]"}},
        {DiagID::InvalidEscapeCharacter,
         {DK_Error,
          "invalid escape character. Only \\\\, \\\", \\n and \\t are allowed"}},
        {DiagID::UnterminatedString, {DK_Error, "unterminated string"}},

        // Parser
        {DiagID::ExpectedToken, {DK_Error, "expected {0}"}},
        {DiagID::UnclosedParenthesis, {DK_Error, "unclosed '('"}},
        {DiagID::MissingUnaryOperand,
         {DK_Error, "missing operand for unary '{0}'"}},
        {DiagID::MissingBinaryOperand,
         {DK_Error, "missing operand for binary '{0}'"}},

        // Semantic
        {DiagID::Redefinition, {DK_Error, "redefinition of '{0}'"}},
        {DiagID::UndefinedType, {DK_Error, "undefined type"}},
        {DiagID::UndefinedName, {DK_Error, "undefined name {0}"}},
        {DiagID::TypeMismatch,
         {DK_Error, "type mismatch: expected: '{0}', got: '{1}'"}},
        {DiagID::UnsupportedBinaryOperandType,
         {DK_Error, "unsupported operand type(s) for '{0}': '{1}' and '{2}'"}},
    };
}

[[nodiscard]] const DiagInfo& getDiagInfo(DiagID diag_id) {
  return table.at(diag_id);
}
