#pragma once

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
  void addError(const llvm::Twine& message, llvm::SMRange location,
                llvm::ArrayRef<llvm::SMRange> ranges,
                llvm::ArrayRef<llvm::SMFixIt> fixits = {});

  /// Add an error diagnostic.
  /// @param message The message to print.
  /// @param location The location where the error occurred.
  /// @param fixits Suggestions on how to fix the error within the line.
  void addError(const llvm::Twine& message, llvm::SMRange location,
                llvm::ArrayRef<llvm::SMFixIt> fixits = {});

  /// Check if an error was found.
  /// @returns Whether an error was found.
  [[nodiscard]] bool hadError() const;

  /// Print all errors.
  void printErrors() const;

  /// Get all diagnostics.
  /// @returns All diagonstics.
  [[nodiscard]] std::span<const llvm::SMDiagnostic> getDiagnostics() const {
    return m_diagnostics;
  }

  /// Clear all diagnostics.
  void clear() { m_diagnostics.clear(); }

private:
  std::vector<llvm::SMDiagnostic> m_diagnostics;
  const llvm::SourceMgr& m_source_manager;
};