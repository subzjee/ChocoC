#include "diagnostics/DiagnosticsManager.h"

void DiagnosticsManager::report(DiagID id, llvm::SMRange location,
                                llvm::SmallVector<std::string> args,
                                llvm::SmallVector<llvm::SMRange> ranges,
                                llvm::SmallVector<llvm::SMFixIt> fixits) {
  m_diagnostics.emplace_back(m_source_manager, id, location, args, ranges,
                             fixits);
}

[[nodiscard]] bool DiagnosticsManager::hadError() const {
  return llvm::any_of(m_diagnostics, [](const auto& diagnostic) {
    return diagnostic.getSeverity() == llvm::SourceMgr::DK_Error;
  });
}

void DiagnosticsManager::printAll() const {
  for (const auto& diagnostic : m_diagnostics) {
    diagnostic.print();
  }
}