#pragma once

#include "llvm/Support/SourceMgr.h"

#include "diagnostics/DiagID.h"

class Diagnostic {
public:
  Diagnostic(DiagID diag_id, llvm::SMDiagnostic diagnostic)
      : m_diag_id(diag_id), m_diagnostic(diagnostic) {}

  /// Get the diagnostic ID of the diagnostic.
  ///
  /// @returns The diagnostic ID.
  [[nodiscard]] DiagID getDiagId() const { return m_diag_id; }

  /// Get the underlying SMDiagnostic.
  ///
  /// @returns The SMDiagnostic.
  [[nodiscard]] const llvm::SMDiagnostic& getSMDiagnostic() const {
    return m_diagnostic;
  }

private:
  const DiagID m_diag_id;
  const llvm::SMDiagnostic m_diagnostic;
};
