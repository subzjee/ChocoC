#pragma once

#include "diagnostics/DiagID.h"
#include "diagnostics/DiagInfo.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/Support/raw_ostream.h"
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
        createFormattedMessage(diag_id, std::move(format_arguments)),
        std::move(ranges), std::move(fixits));
  }

  /// Get the diagnostic ID of the diagnostic.
  /// @return The diagnostic ID.
  [[nodiscard]] DiagID getDiagId() const { return m_diag_id; }

  /// Get the severity of the diagnostic.
  /// @returns The severity.
  [[nodiscard]] llvm::SourceMgr::DiagKind getSeverity() const {
    return m_severity;
  }

  /// Get the formatted message of the diagnostic.
  /// @returns The formatted message.
  [[nodiscard]] llvm::StringRef getFormattedMessage() const { return m_diagnostic.getMessage(); }

  /// Get the formatted message of a diagnostic ID's message.
  /// It will format based on index, where the argument's index is given as {idx}.
  /// For example: "{1} and {0}" with {"a", "b"} will return "b and a".
  /// If the index does not exist in \p format_args, it will simply append it verbatim.
  /// @param diag_id The diagnostic ID.
  /// @param format_args The format arguments.
  /// @returns The formatted message.
  [[nodiscard]] static std::string
  createFormattedMessage(DiagID diag_id, llvm::ArrayRef<std::string> format_args) {
    const auto& diag_info = getDiagInfo(diag_id);

    std::string result;
    size_t i = 0;

    while (i < diag_info.format.size()) {
      if (diag_info.format[i] == '{' &&
          i + 1 < diag_info.format.size() - 1 && llvm::isDigit(diag_info.format[i+1]) && diag_info.format[i + 2] == '}') {
        std::size_t arg_index = diag_info.format[i + 1] - '0';

        if (arg_index < format_args.size()) {
          result += format_args[arg_index];
        } else {
          result += "{";
          result += diag_info.format[i+1];
          result += '}';
        }

        i += 3;
      } else {
        result += diag_info.format[i++];
      }
    }

    return result;
  }

  /// Print the diagnostic.
  void print() const { m_diagnostic.print("", llvm::errs()); }

private:
  DiagID m_diag_id;
  llvm::SourceMgr::DiagKind m_severity;
  llvm::SMDiagnostic m_diagnostic;
};