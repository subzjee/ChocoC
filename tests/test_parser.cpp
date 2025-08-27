#include "ast/PrettyPrinter.h"
#include "diagnostics/DiagnosticsManager.h"
#include "lexer/Lexer.h"
#include "parser/Parser.h"

#include "gtest/gtest.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/MemoryBuffer.h"

#include <filesystem>

using namespace chocopy;
using namespace chocopy::parser;

class ParserTest : public testing::Test, public testing::WithParamInterface<std::string> {
protected:
  llvm::SourceMgr source_manager;
  DiagnosticsManager diagnostics_manager{source_manager};
  ast::PrettyPrinter pretty_printer{};

  void TearDown() override { diagnostics_manager.clear(); }
};

TEST_P(ParserTest, Parser) {
  auto input_file = llvm::MemoryBuffer::getFile("tests/input/parser/" + GetParam() + ".choco");
  auto golden_file = llvm::MemoryBuffer::getFile("tests/input/parser/" + GetParam() + ".golden");

  unsigned int buffer_id =
      source_manager.AddNewSourceBuffer(std::move(input_file.get()), llvm::SMLoc());
  Lexer lexer{buffer_id, source_manager, diagnostics_manager};
  const auto tokens = lexer.lex();
  TokenStream token_stream{tokens};

  Parser parser{token_stream, diagnostics_manager};

  const auto root = parser.parse();
  ASSERT_NE(root, nullptr);

  const std::string pretty_print = std::any_cast<std::string>(root->accept(pretty_printer));
  
  EXPECT_EQ(pretty_print, (std::string{golden_file->get()->getBufferStart(), golden_file->get()->getBufferEnd()}));
}

static const std::vector<std::string> test_names = {
  "vardefs",
  "binary_ops",
  "unary_ops",
  "operator_precedence"
};

INSTANTIATE_TEST_SUITE_P(
  ParserFileTests,
  ParserTest,
  testing::ValuesIn(test_names),
  [](const testing::TestParamInfo<std::string>& param_info) {
    return param_info.param;
  }
);