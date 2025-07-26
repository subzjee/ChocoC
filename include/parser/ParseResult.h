#pragma once

#include "llvm/Support/SourceMgr.h"

#include <expected>
#include <memory>

template <typename ParseRule>
using ParseResult = std::expected<std::unique_ptr<ParseRule>, std::string>;