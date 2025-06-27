add_library(chocopc_lib
    ASTVisitor.cpp
    DiagnosticsManager.cpp
    ast/AssignmentStatement.cpp
    ast/BinaryExpression.cpp
    ast/Literal.cpp
    ast/PrettyPrinter.cpp
    ast/Program.cpp
    ast/Target.cpp
    ast/Type.cpp
    ast/VariableDefinition.cpp
    ir/LLVM.cpp
    lexer/Lexer.cpp
    parser/Parser.cpp
    semantic/SymbolTable.cpp
    semantic/SymbolTableBuilder.cpp
    semantic/TypeChecker.cpp
    semantic/TypeEnvironment.cpp
)

target_link_libraries(chocopc_lib PUBLIC c++abi ${llvm_libs})
target_include_directories(chocopc_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(chocopc chocopc.cpp)

target_link_libraries(chocopc chocopc_lib)