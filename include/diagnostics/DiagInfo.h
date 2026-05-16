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

[[nodiscard]] const DiagInfo& getDiagInfo(DiagID diag_id);
