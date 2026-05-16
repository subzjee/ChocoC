#pragma once

#include <initializer_list>
#include <span>
#include <vector>

#include "llvm/Support/FormatVariadic.h"
#include "llvm/Support/SourceMgr.h"

#include "diagnostics/DiagID.h"
#include "diagnostics/DiagInfo.h"
#include "diagnostics/Diagnostic.h"

class DiagnosticsManager {
public:
  class DiagnosticBuilder;

  explicit DiagnosticsManager(const llvm::SourceMgr& source_manager)
      : m_source_manager(source_manager) {};

  /// Create a new diagnostic with a given diagnostic ID.
  ///
  /// @param diag_id The diagnostic ID.
  ///
  /// @returns A DiagnosticBuilder representing the new diagnostic.
  DiagnosticBuilder report(DiagID diag_id);

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

class DiagnosticsManager::DiagnosticBuilder {
public:
  DiagnosticBuilder(DiagnosticsManager& manager, DiagID diag_id)
      : m_manager(manager), m_diag_id(diag_id),
        m_message(getFormat(diag_id)) {};

  DiagnosticBuilder(const DiagnosticBuilder&) = delete;
  DiagnosticBuilder& operator=(const DiagnosticBuilder&) = delete;
  DiagnosticBuilder(DiagnosticBuilder&&) = delete;
  DiagnosticBuilder& operator=(DiagnosticBuilder&&) = delete;

  ~DiagnosticBuilder() {
    // Automatically emit on destruction so diagnostics don't get lost.
    if (!m_has_emitted) {
      emit();
    }
  }

  /// Set the location for the diagnostic.
  ///
  /// @param location The location.
  ///
  /// @returns The DiagnosticBuilder representing the diagnostic.
  DiagnosticBuilder& at(llvm::SMLoc location) {
    m_loc = location;

    return *this;
  }

  /// Set the arguments for the diagnostic's message format.
  ///
  /// @param args The arguments.
  ///
  /// @returns The DiagnosticBuilder representing the diagnostic.
  template <typename... Args>
  DiagnosticBuilder& args(Args&&... args) {
    m_message = llvm::formatv(true, getFormat(m_diag_id).data(),
                              std::forward<Args>(args)...)
                    .str();

    return *this;
  }

  /// Add an underline range within the diagnostic message.
  ///
  /// @param range The range.
  ///
  /// @returns The DiagnosticBuilder representing the diagnostic.
  DiagnosticBuilder& range(llvm::SMRange range) {
    m_ranges.push_back(range);

    return *this;
  }

  /// Add underline ranges within the diagnostic message.
  ///
  /// @param ranges The ranges.
  ///
  /// @returns The DiagnosticBuilder representing the diagnostic.
  DiagnosticBuilder& range(std::initializer_list<llvm::SMRange> ranges) {
    for (auto& range : ranges) {
      m_ranges.push_back(range);
    }

    return *this;
  }

  /// Add a fix-it hints to the diagnostic message.
  ///
  /// @param fixit The fix-it hint
  ///
  /// @returns The DiagnosticBuilder representing the diagnostic.
  DiagnosticBuilder& fixit(llvm::SMFixIt fixit) {
    m_fixits.push_back(fixit);

    return *this;
  }

  /// Add fix-it hints to the diagnostic message.
  ///
  /// @param fixits The fix-it hints.
  ///
  /// @returns The DiagnosticBuilder representing the diagnostic.
  DiagnosticBuilder& fixit(std::initializer_list<llvm::SMFixIt> fixits) {
    for (auto& fixit : fixits) {
      m_fixits.push_back(fixit);
    }

    return *this;
  }

  /// Emit the diagnostic to the diagnostic manager.
  void emit() {
    auto diag = m_manager.m_source_manager.GetMessage(
        m_loc, getSeverity(m_diag_id), std::move(m_message),
        std::move(m_ranges), std::move(m_fixits));

    m_manager.m_diagnostics.emplace_back(m_diag_id, diag);
    m_has_emitted = true;
  }

private:
  DiagnosticsManager& m_manager;
  DiagID m_diag_id;
  bool m_has_emitted = false;

  llvm::SMLoc m_loc;
  std::string m_message;
  llvm::SmallVector<llvm::SMRange> m_ranges;
  llvm::SmallVector<llvm::SMFixIt> m_fixits;
};
