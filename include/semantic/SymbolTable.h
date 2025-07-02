#pragma once

#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Type.h"

#include <variant>

namespace chocopy {
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

struct Variable {
  Variable(const llvm::StringRef name, const Type& type)
      : name(name), type(type), allocation(nullptr) {};

  const llvm::StringRef name;
  const Type& type;
  llvm::Value* allocation;
};

using SymbolTableEntry = std::variant<Variable, Type>;
class SymbolTable {
public:
  /// Add an entry to the symbol table.
  /// @param name The name of the symbol.
  /// @param entry The symbol table entry.
  /// @returns The entry and a success value as a pair.
  std::pair<llvm::StringMapIterator<SymbolTableEntry>, bool>
  addEntry(const llvm::StringRef name, const SymbolTableEntry& entry);

  /// Get an entry from the symbol table.
  /// @param name The name of the symbol to get.
  /// @returns The symbol.
  std::optional<std::reference_wrapper<SymbolTableEntry>>
  getEntry(const llvm::StringRef name);

  /// Get all entries from the symbol table.
  /// @returns All entries.
  llvm::StringMap<SymbolTableEntry> getAllEntries() const { return m_entries; }

private:
  llvm::StringMap<SymbolTableEntry> m_entries = {
      {"object", *Type::getObjectType()}, {"int", *Type::getIntegerType()},
      {"str", *Type::getStringType()},    {"bool", *Type::getBooleanType()},
      {"<None>", *Type::getNoneType()},   {"<Empty>", *Type::getEmptyType()}};
};
} // namespace chocopy