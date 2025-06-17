#pragma once

#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringRef.h"

#include <variant>

namespace chocopy {
struct Type {
  Type(const llvm::StringRef base_type, const unsigned int dimension = 0)
      : base_type(base_type), dimension(dimension) {};

  constexpr bool operator==(const Type& other) const = default;

  constexpr bool isList() const { return dimension >= 1; }
  constexpr bool isEmpty() const { return base_type == "<Empty>"; }
  constexpr bool isNone() const { return base_type == "<None>" && !isList(); }
  constexpr bool isInteger() const { return base_type == "int" && !isList(); }
  constexpr bool isBoolean() const { return base_type == "bool" && !isList(); }
  constexpr bool isString() const { return base_type == "str" && !isList(); }

  [[nodiscard]] constexpr std::string toString() const {
    return std::string(dimension, '[') + base_type.str() +
           std::string(dimension, ']');
  };

  const llvm::StringRef base_type;
  const unsigned int dimension;
};

struct Variable {
  Variable(const llvm::StringRef name, const Type& type)
      : name(name), type(type) {};

  const llvm::StringRef name;
  const Type type;
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
  std::optional<SymbolTableEntry> getEntry(const llvm::StringRef name) const;

  /// Get all entries from the symbol table.
  /// @returns All entries.
  llvm::StringMap<SymbolTableEntry> getAllEntries() const { return m_entries; }

private:
  llvm::StringMap<SymbolTableEntry> m_entries = {
      {"object", Type{"object"}}, {"int", Type{"int"}},
      {"str", Type{"str"}},       {"bool", Type{"bool"}},
      {"<None>", Type{"<None>"}}, {"<Empty>", Type{"<Empty>"}}};
};
} // namespace chocopy