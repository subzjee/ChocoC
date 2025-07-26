#pragma once

#include "llvm/ADT/StringRef.h"

#include <memory>

namespace chocopy::sema {
struct Type {
  Type(const llvm::StringRef base_type, const unsigned int dimension = 0,
       const std::shared_ptr<Type> super_class = nullptr)
      : base_type(base_type), dimension(dimension), super_class(super_class) {};

  constexpr bool operator==(const Type& other) const = default;

  constexpr bool isList() const { return dimension >= 1; }
  constexpr bool isEmpty() const { return *this == *Type::getEmptyType(); }
  constexpr bool isNone() const { return *this == *Type::getNoneType(); }
  constexpr bool isInteger() const { return *this == *Type::getIntegerType(); }
  constexpr bool isBoolean() const { return *this == *Type::getBooleanType(); }
  constexpr bool isString() const { return *this == *Type::getStringType(); }

  static std::shared_ptr<Type> getObjectType() {
    static const auto type = std::make_shared<Type>("object", 0);
    return type;
  }

  static std::shared_ptr<Type> getIntegerType() {
    static const auto type = std::make_shared<Type>("int", 0, getObjectType());
    return type;
  }

  static std::shared_ptr<Type> getStringType() {
    static const auto type = std::make_shared<Type>("str", 0, getObjectType());
    return type;
  }

  static std::shared_ptr<Type> getBooleanType() {
    static const auto type = std::make_shared<Type>("bool", 0, getObjectType());
    return type;
  }

  static std::shared_ptr<Type> getNoneType() {
    static const auto type =
        std::make_shared<Type>("<None>", 0, getObjectType());
    return type;
  }

  static std::shared_ptr<Type> getEmptyType() {
    static const auto type =
        std::make_shared<Type>("<Empty>", 0, getObjectType());
    return type;
  }

  [[nodiscard]] std::string toString() const {
    return std::string(dimension, '[') + base_type.str() +
           std::string(dimension, ']');
  };

  const llvm::StringRef base_type;
  const unsigned int dimension;
  const std::shared_ptr<Type> super_class;
};
}