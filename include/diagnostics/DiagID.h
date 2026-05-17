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
  ExpectedNewLine,
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