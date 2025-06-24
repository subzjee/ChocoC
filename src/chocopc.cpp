#include "DiagnosticsManager.h"
#include "ir/LLVM.h"
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "semantic/SymbolTableBuilder.h"
#include "semantic/TypeChecker.h"

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace chocopy;

static cl::list<std::string> input_file_names(cl::Positional, cl::desc("<input_files>"), cl::OneOrMore);
static cl::opt<std::string> output_file_name("o", cl::desc("Output filename"), cl::value_desc("filename"));

int main(int argc, char* argv[]) {
  cl::ParseCommandLineOptions(argc, argv);

  SourceMgr source_manager;

  for (const auto& input_file_name : input_file_names) {
    DiagnosticsManager diag_manager{source_manager};
    auto file = MemoryBuffer::getFile(input_file_name);

    if (!file) {
      errs() << std::system_error(errno, std::system_category(),
                            "error: " + input_file_name).what() << '\n';
      continue;
    }

    unsigned int buffer_id = source_manager.AddNewSourceBuffer(std::move(file.get()), SMLoc());

    /* Lexical analysis */
    Lexer lexer{buffer_id, source_manager, diag_manager};
    auto tokens = lexer.lex();

    if (diag_manager.hadError()) {
      diag_manager.printErrors();
      continue;
    }

    /* Syntax analysis */
    Parser parser{tokens, diag_manager};
    const auto root = parser.parse();

    if (diag_manager.hadError()) {
      diag_manager.printErrors();
      continue;
    }

    /* Semantic analysis */
    SymbolTableBuilder builder{diag_manager};
    builder.visit(*root);
    auto& symbol_table = builder.getSymbolTable();

    TypeChecker type_checker{symbol_table, diag_manager};
    type_checker.visit(*root);

    if (diag_manager.hadError()) {
      diag_manager.printErrors();
      continue;
    }

    /* IR Generation */
    IRGen code_gen{input_file_name, symbol_table};
    code_gen.visit(*root);
  }

  return 0;
}