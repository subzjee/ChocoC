#pragma once

#include "diagnostics/DiagID.h"
#include "diagnostics/DiagInfo.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/Support/FormatVariadic.h"
#include "llvm/Support/SourceMgr.h"

class Diagnostic {
public:
  Diagnostic(const llvm::SourceMgr& source_manager, DiagID diag_id,
             llvm::SMRange location,
             llvm::SmallVector<std::string> format_arguments = {},
             llvm::SmallVector<llvm::SMRange> ranges = {},
             llvm::SmallVector<llvm::SMFixIt> fixits = {})
      : m_diag_id(diag_id) {
    const auto& [severity, message] = getDiagInfo(diag_id);
    m_severity = severity;
    m_diagnostic = source_manager.GetMessage(
        location.Start, severity,
        getFormattedMessage(diag_id, std::move(format_arguments)),
        std::move(ranges), std::move(fixits));
  }

  [[nodiscard]] DiagID getDiagId() const { return m_diag_id; }
  [[nodiscard]] llvm::SourceMgr::DiagKind getSeverity() const {
    return m_severity;
  }

  [[nodiscard]] static std::string
  getFormattedMessage(DiagID diag_id, llvm::ArrayRef<std::string> format_args) {
    const auto& diag_info = getDiagInfo(diag_id);

    std::string result;
    size_t i = 0, arg_index = 0;

    while (i < diag_info.format.size()) {
      if (arg_index < format_args.size() && diag_info.format[i] == '{' &&
          i + 1 < diag_info.format.size() && diag_info.format[i + 2] == '}') {
        result += format_args[arg_index++];
        i += 3;
      } else {
        result += diag_info.format[i++];
      }
    }

    return result;
  }

  void print() const { m_diagnostic.print("", llvm::errs()); }

private:
  DiagID m_diag_id;
  llvm::SourceMgr::DiagKind m_severity;
  llvm::SMDiagnostic m_diagnostic;
};