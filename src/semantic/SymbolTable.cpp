#include "semantic/SymbolTable.h"
#include <llvm-21/llvm/ADT/StringMap.h>

namespace chocopy {
std::pair<llvm::StringMapIterator<SymbolTableEntry>, bool> SymbolTable::addEntry(const llvm::StringRef name, const SymbolTableEntry& entry) {
  return m_entries.try_emplace(name, entry);
}

std::optional<SymbolTableEntry> SymbolTable::getEntry(const llvm::StringRef name) const {
  const auto it = m_entries.find(name);

  if (it == m_entries.end()) {
    return std::nullopt;
  }

  return it->second;
}
} // namespace chocopy