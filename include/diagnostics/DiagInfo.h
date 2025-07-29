#pragma once

#include "diagnostics/DiagID.h"

#include "llvm/Support/SourceMgr.h"

struct DiagInfo {
  constexpr DiagInfo(llvm::SourceMgr::DiagKind severity,
                     std::string_view format)
      : severity{severity}, format{format} {};

  llvm::SourceMgr::DiagKind severity;
  std::string_view format;
};

[[nodiscard]] const DiagInfo& getDiagInfo(DiagID diag_id);