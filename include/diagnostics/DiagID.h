#pragma once

enum class DiagID {
  /* Lexer */
  UnexpectedCharacter,
  LeadingZeros,
  IntegerOutOfRange,
  InvalidEscapeCharacter,
  UnterminatedString,

  /* Parser */
  ExpectedToken, // Generic `expected {0}` format.
  UnclosedParenthesis,
  MissingUnaryOperand,
  MissingBinaryOperand,

  /* Semantic */
  Redefinition,
  UndefinedType,
  UndefinedName,
  TypeMismatch,
  UnsupportedBinaryOperandType
};