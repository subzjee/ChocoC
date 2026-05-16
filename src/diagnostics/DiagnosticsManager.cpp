#include "diagnostics/DiagnosticsManager.h"

#include <diagnostics/DiagInfo.h>

[[nodiscard]] DiagnosticsManager::DiagnosticBuilder
DiagnosticsManager::report(DiagID diag_id) {
  return {*this, diag_id};
}

[[nodiscard]] bool DiagnosticsManager::hadError() const {
  return llvm::any_of(m_diagnostics, [](const Diagnostic& diagnostic) {
    return getSeverity(diagnostic.getDiagId()) == llvm::SourceMgr::DK_Error;
  });
}

void DiagnosticsManager::printAll() const {
  for (const auto& diagnostic : m_diagnostics) {
    diagnostic.getSMDiagnostic().print("", llvm::errs());
  }
}
