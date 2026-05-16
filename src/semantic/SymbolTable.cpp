#include "semantic/SymbolTable.h"

namespace chocopy {
void SymbolTable::addEntry(const llvm::StringRef name,
                           const SymbolTableEntry& entry) {
  m_entries.try_emplace(name, entry);
}

OptionalRef<SymbolTableEntry>
SymbolTable::getEntry(const llvm::StringRef name) {
  auto it = m_entries.find(name);

  if (it == m_entries.end()) {
    return std::nullopt;
  }

  return it->second;
}
} // namespace chocopy
