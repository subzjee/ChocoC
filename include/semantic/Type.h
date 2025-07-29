#pragma once

#include "llvm/ADT/StringRef.h"

#include <memory>

namespace chocopy::sema {
struct Type {
  Type(const llvm::StringRef base_type, const unsigned int dimension = 0,
       const std::shared_ptr<Type> super_class = nullptr)
      : base_type(base_type), dimension(dimension), super_class(super_class) {};

  constexpr bool operator==(const Type& other) const = default;

  /// Check if the type is equivalent to the <Empty> type.
  /// @returns Whether the type is equivalent to the Empty type.
  constexpr bool isEmpty() const { return *this == *Type::getEmptyType(); }

  /// Check if the type is equivalent to the <None> type.
  /// @returns Whether the type is equivalent to the Empty type.
  constexpr bool isNone() const { return *this == *Type::getNoneType(); }

  /// Check if the type is equivalent to the `int` type.
  /// @returns Whether the type is equivalent to the Empty type.
  constexpr bool isInteger() const { return *this == *Type::getIntegerType(); }

  /// Check if the type is equivalent to the `bool` type.
  /// @returns Whether the type is equivalent to the Empty type.
  constexpr bool isBoolean() const { return *this == *Type::getBooleanType(); }

  /// Check if the type is equivalent to the `str` type.
  /// @returns Whether the type is equivalent to the Empty type.
  constexpr bool isString() const { return *this == *Type::getStringType(); }

  /// Check if the type is a list.
  /// @returns Whether the type is a list.
  constexpr bool isList() const { return dimension >= 1; }

  /// Check whether the type is a subclass of \p type.
  /// @param type The parent class.
  /// @returns Whether the type is a subclass of \p type.
  [[nodiscard]] bool isSubclass(const Type& type) const;

  /// Check whether the type conforms to \p type.
  /// @param first The type to check for conformity.
  /// @param second The type to check against.
  /// @returns Whether the type conforms to \p type.
  [[nodiscard]] bool conformsTo(const Type& type) const;

  /// Check whether the type is assignment-compatible with \p type.
  /// @param first The type to check for assignment compatibility.
  /// @param second The type to check against.
  /// @returns Whether the type is assignment-compatible with \p type.
  [[nodiscard]] bool isAssignmentCompatible(const Type& type) const;

  /// Get the `object` type.
  /// @returns The `object` type.
  static std::shared_ptr<Type> getObjectType() {
    static const auto type = std::make_shared<Type>("object", 0);
    return type;
  }

  /// Get the `int` type.
  /// @returns The `int` type.
  static std::shared_ptr<Type> getIntegerType() {
    static const auto type = std::make_shared<Type>("int", 0, getObjectType());
    return type;
  }

  /// Get the `str` type.
  /// @returns The `str` type.
  static std::shared_ptr<Type> getStringType() {
    static const auto type = std::make_shared<Type>("str", 0, getObjectType());
    return type;
  }

  /// Get the `bool` type.
  /// @returns The `bool` type.
  static std::shared_ptr<Type> getBooleanType() {
    static const auto type = std::make_shared<Type>("bool", 0, getObjectType());
    return type;
  }

  /// Get the `<None>` type.
  /// @returns The `<None>` type.
  static std::shared_ptr<Type> getNoneType() {
    static const auto type =
        std::make_shared<Type>("<None>", 0, getObjectType());
    return type;
  }

  /// Get the `<Empty>` type.
  /// @returns The `<Empty>` type.
  static std::shared_ptr<Type> getEmptyType() {
    static const auto type =
        std::make_shared<Type>("<Empty>", 0, getObjectType());
    return type;
  }

  /// Get the string representation of the type.
  /// @returns The string representation.
  [[nodiscard]] std::string toString() const {
    return std::string(dimension, '[') + base_type.str() +
           std::string(dimension, ']');
  };

  const llvm::StringRef base_type;
  const unsigned int dimension;
  const std::shared_ptr<Type> super_class;
};
} // namespace chocopy::sema