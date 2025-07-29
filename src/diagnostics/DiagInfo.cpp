#include "diagnostics/DiagInfo.h"

[[nodiscard]] const DiagInfo& getDiagInfo(DiagID diag_id) {
  /* Lexer */
  static constexpr DiagInfo unexpected_character{llvm::SourceMgr::DK_Error,
                                                 "unexpected character"};
  static constexpr DiagInfo leading_zeros{
      llvm::SourceMgr::DK_Error, "an integer may not have leading zeros"};
  static constexpr DiagInfo integer_out_of_range{
      llvm::SourceMgr::DK_Error,
      "an integer must be within the range [-2147483648, 2147483647]"};
  static constexpr DiagInfo invalid_escape_character{
      llvm::SourceMgr::DK_Error, "invalid escape character. Only \\\\, \\\", "
                                 "\\n and \\t are allowed"};
  static constexpr DiagInfo unterminated_string{llvm::SourceMgr::DK_Error,
                                                "unterminated string"};

  /* Parser */
  static constexpr DiagInfo expected_token{llvm::SourceMgr::DK_Error,
                                           "expected {0}"};
  static constexpr DiagInfo unclosed_parenthesis{llvm::SourceMgr::DK_Error,
                                                 "unclosed '("};
  static constexpr DiagInfo missing_unary_operand{
      llvm::SourceMgr::DK_Error, "missing operand for unary '{0}'"};

  /* Semantic */
  static constexpr DiagInfo redefinition{llvm::SourceMgr::DK_Error,
                                         "redefinition of '{0}'"};
  static constexpr DiagInfo undefined_type{llvm::SourceMgr::DK_Error,
                                           "undefined type"};
  static constexpr DiagInfo undefined_name{llvm::SourceMgr::DK_Error,
                                           "undefined name {0}"};
  static constexpr DiagInfo type_mismatch{
      llvm::SourceMgr::DK_Error, "type mismatch: expected: '{0}', got: '{1}'"};
  static constexpr DiagInfo unsupported_binary_operand_type{
      llvm::SourceMgr::DK_Error,
      "unsupported operand type(s) for '{0}': '{1}' and '{2}'"};

  switch (diag_id) {
  /* Lexer */
  case DiagID::UnexpectedCharacter:
    return unexpected_character;
  case DiagID::LeadingZeros:
    return leading_zeros;
  case DiagID::IntegerOutOfRange:
    return integer_out_of_range;
  case DiagID::InvalidEscapeCharacter:
    return invalid_escape_character;
  case DiagID::UnterminatedString:
    return unterminated_string;

  /* Parser */
  case DiagID::ExpectedToken:
    return expected_token;
  case DiagID::UnclosedParenthesis:
    return unclosed_parenthesis;
  case DiagID::MissingUnaryOperand:
    return missing_unary_operand;

  /* Semantic */
  case DiagID::Redefinition:
    return redefinition;
  case DiagID::UndefinedType:
    return undefined_type;
  case DiagID::UndefinedName:
    return undefined_name;
  case DiagID::TypeMismatch:
    return type_mismatch;
  case DiagID::UnsupportedBinaryOperandType:
    return unsupported_binary_operand_type;

  default:
    std::unreachable();
  }
}