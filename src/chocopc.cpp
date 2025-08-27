#include "ast/PrettyPrinter.h"
#include "diagnostics/DiagnosticsManager.h"
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

int main(int argc, char* argv[]) {
  cl::list<std::string> input_file_names(
      cl::Positional, cl::desc("<input_files>"), cl::OneOrMore);
  cl::opt<std::string> output_file_name("o", cl::desc("Output filename"),
                                        cl::value_desc("filename"));
  cl::opt<bool> do_emit_llvm("emit-llvm", cl::desc("Emit LLVM IR"));
  cl::opt<bool> do_pprint("pretty-print-ast",
                          cl::desc("Emit a pretty print of the AST"));

  cl::ParseCommandLineOptions(argc, argv);

  SourceMgr source_manager;

  for (const auto& input_file_name : input_file_names) {
    DiagnosticsManager diag_manager{source_manager};
    auto file = MemoryBuffer::getFile(input_file_name);

    if (!file) {
      errs() << std::system_error(errno, std::system_category(),
                                  "error: " + input_file_name)
                    .what()
             << '\n';
      continue;
    }

    unsigned int buffer_id =
        source_manager.AddNewSourceBuffer(std::move(file.get()), SMLoc());

    /* Lexical analysis */
    Lexer lexer{buffer_id, source_manager, diag_manager};
    auto tokens = lexer.lex();

    diag_manager.printAll();
    if (diag_manager.hadError()) {
      continue;
    }

    TokenStream token_stream{tokens};

    /* Syntax analysis */
    parser::Parser parser{token_stream, diag_manager};
    const auto root = parser.parse();

    diag_manager.printAll();
    if (!root || diag_manager.hadError()) {
      continue;
    }

    if (do_pprint) {
      ast::PrettyPrinter pretty_printer{};
      const std::string pretty_print =
          std::any_cast<std::string>(root->accept(pretty_printer));
      llvm::outs() << pretty_print << '\n';
    }

    /* Semantic analysis */
    SymbolTableBuilder builder{diag_manager};
    root->accept(builder);
    auto& symbol_table = builder.getSymbolTable();

    TypeChecker type_checker{symbol_table, diag_manager};
    root->accept(type_checker);

    diag_manager.printAll();
    if (diag_manager.hadError()) {
      continue;
    }

    /* IR Generation */
    IRGen code_gen{input_file_name, symbol_table};
    root->accept(code_gen);

    if (do_emit_llvm) {
      code_gen.getModule().print(llvm::outs(), nullptr);
    }
  }

  return 0;
}