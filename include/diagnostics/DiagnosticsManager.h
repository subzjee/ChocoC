#pragma once

#include "diagnostics/DiagID.h"
#include "diagnostics/Diagnostic.h"

#include "llvm/Support/SourceMgr.h"

#include <vector>

class DiagnosticsManager {
public:
  DiagnosticsManager(const llvm::SourceMgr& source_manager)
      : m_source_manager(source_manager) {};

  /// Add an error diagnostic.
  /// @param message The message to print.
  /// @param location The location where the error occurred.
  /// @param ranges Ranges of columns that should be underlined.
  /// @param fixits Suggestions on how to fix the error within the line.
  void report(DiagID id, llvm::SMRange location,
              llvm::SmallVector<std::string> args = {},
              llvm::SmallVector<llvm::SMRange> ranges = {},
              llvm::SmallVector<llvm::SMFixIt> fixits = {});

  /// Check if an error was found.
  /// @returns Whether an error was found.
  [[nodiscard]] bool hadError() const;

  /// Print all diagnostics.
  void printAll() const;

  /// Get all diagnostics.
  /// @returns All diagonstics.
  [[nodiscard]] std::span<const Diagnostic> getDiagnostics() const {
    return m_diagnostics;
  }

  /// Clear all diagnostics.
  void clear() { m_diagnostics.clear(); }

private:
  std::vector<Diagnostic> m_diagnostics;
  const llvm::SourceMgr& m_source_manager;
};