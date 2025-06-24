#include "DiagnosticsManager.h"

using namespace llvm;

void DiagnosticsManager::addError(const Twine& message, SMRange location, ArrayRef<SMRange> ranges, ArrayRef<SMFixIt> fixits) {
  SMDiagnostic diagnostic = m_source_manager.GetMessage(location.Start, SourceMgr::DK_Error, message,
                                ranges, fixits);

  m_diagnostics.push_back(diagnostic);
}

void DiagnosticsManager::addError(const Twine& message, SMRange location, ArrayRef<SMFixIt> fixits) {
  addError(message, location, {location}, fixits);
}

void DiagnosticsManager::printErrors() const {
  for (const auto& diagnostic : m_diagnostics) {
    diagnostic.print("", llvm::errs());
  }
}