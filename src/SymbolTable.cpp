#include "SymbolTable.h"

#include "llvm/ADT/StringMap.h"

namespace chocopy {
std::pair<llvm::StringMapIterator<SymbolTableEntry>, bool> SymbolTable::addEntry(const llvm::StringRef name, const SymbolTableEntry& entry) {
  return m_entries.try_emplace(name, entry);
}

std::optional<std::reference_wrapper<SymbolTableEntry>> SymbolTable::getEntry(const llvm::StringRef name) {
  auto it = m_entries.find(name);

  if (it == m_entries.end()) {
    return std::nullopt;
  }

  return it->second;
}
} // namespace chocopy