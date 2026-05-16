add_library(libchoco
    TokenStream.cpp
    ast/AssignmentStatement.cpp
    ast/ASTVisitor.cpp
    ast/Block.cpp
    ast/BinaryExpression.cpp
    ast/GroupingExpression.cpp
    ast/Identifier.cpp
    ast/Literal.cpp
    ast/PrettyPrinter.cpp
    ast/Program.cpp
    ast/Type.cpp
    ast/UnaryExpression.cpp
    ast/VariableDefinition.cpp
    diagnostics/DiagInfo.cpp
    diagnostics/DiagnosticsManager.cpp
    ir/LLVM.cpp
    lexer/Lexer.cpp
    parser/Parser.cpp
    semantic/SymbolTable.cpp
    semantic/SymbolTableBuilder.cpp
    semantic/Type.cpp
    semantic/TypeChecker.cpp
    semantic/TypeEnvironment.cpp
)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

llvm_map_components_to_libnames(llvm_libs support core irreader)

set_target_properties(libchoco PROPERTIES PREFIX "")
target_link_libraries(libchoco PUBLIC ${llvm_libs})

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
target_compile_definitions(libchoco PUBLIC ${LLVM_DEFINITIONS_LIST})

target_include_directories(libchoco
    SYSTEM AFTER PUBLIC
        ${LLVM_INCLUDE_DIRS}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

add_executable(chococ chocopc.cpp)

target_link_libraries(chococ PRIVATE libchoco)
