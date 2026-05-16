#pragma once

#include "llvm/Support/SourceMgr.h"

#include "diagnostics/DiagID.h"

struct DiagInfo {
  constexpr DiagInfo(llvm::SourceMgr::DiagKind severity,
                     std::string_view format)
      : severity{severity}, format{format} {};

  llvm::SourceMgr::DiagKind severity;
  std::string_view format;
};

/// Get the severity of a diagnostic ID.
///
/// @param The diagnostic ID.
///
/// @returns The severity.
[[nodiscard]] const llvm::SourceMgr::DiagKind& getSeverity(DiagID diag_id);

/// Get the severity of a diagnostic ID.
///
/// @param The diagnostic ID.
///
/// @returns The format.
[[nodiscard]] std::string_view getFormat(DiagID diag_id);

/// Get the diagnostic info of a diagnostic ID.
///
/// @param The diagnostic ID.
///
/// @returns The diagnostic info.
[[nodiscard]] const DiagInfo& getDiagInfo(DiagID diag_id);
