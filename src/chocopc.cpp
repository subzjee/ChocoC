#include "lexer/Lexer.h"

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
    auto file = MemoryBuffer::getFile(input_file_name);

    if (!file) {
      errs() << std::system_error(errno, std::system_category(),
                            "error: " + input_file_name).what() << '\n';
      continue;
    }

    unsigned int buffer_id = source_manager.AddNewSourceBuffer(std::move(file.get()), SMLoc());

    Lexer lexer{buffer_id, source_manager};

    auto tokens = lexer.lex();
    bool had_error = false;

    for (const auto& diagnostic : lexer.getDiagnostics()) {
      had_error |= diagnostic.getKind() == SourceMgr::DK_Error;
      diagnostic.print("", errs());
    }

    if (had_error) {
      continue;
    }
  }

  return 0;
}