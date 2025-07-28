#pragma once

#include "semantic/Type.h"
#include "semantic/Variable.h"
#include "utility/OptionalRef.h"

#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Type.h"

#include <variant>

namespace chocopy {
using namespace sema;

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
  OptionalRef<SymbolTableEntry>
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