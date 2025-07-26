#pragma once

#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Type.h"

namespace chocopy {
template <typename Entry>
class SymbolTable {
public:
  /// Add an entry to the symbol table.
  /// @param name The name of the symbol.
  /// @param entry The symbol table entry.
  /// @returns The entry and a success value as a pair.
  std::pair<llvm::StringMapIterator<Entry>, bool>
  addEntry(const llvm::StringRef name, const Entry& entry) {
    return m_entries.try_emplace(name, entry);
  }

  /// Get an entry from the symbol table.
  /// @param name The name of the symbol to get.
  /// @returns The symbol.
  std::optional<std::reference_wrapper<Entry>>
  getEntry(const llvm::StringRef name) {
    auto it = m_entries.find(name);

    if (it == m_entries.end()) {
      return std::nullopt;
    }

    return it->second;
  }

  /// Check if the symbol table contains an entry with key \p name.
  /// @param name The name of the symbol to check.
  /// @returns Whether the symbol exists.
  bool hasEntry(const llvm::StringRef name) { return m_entries.contains(name ); }

  /// Get all entries from the symbol table.
  /// @returns All entries.
  llvm::StringMap<Entry> getAllEntries() const { return m_entries; }

private:
  llvm::StringMap<Entry> m_entries = {};
};
} // namespace chocopy