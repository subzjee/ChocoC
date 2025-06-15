#include "DiagnosticsManager.h"

using namespace llvm;

void DiagnosticsManager::addError(const Twine& message, SMRange location, ArrayRef<SMFixIt> fixits) {
  SMDiagnostic diagnostic = m_source_manager.GetMessage(location.Start, SourceMgr::DK_Error, message,
                                {SMRange{location.Start, location.End}}, fixits);

  m_diagnostics.push_back(diagnostic);
}