add_library(chocopc_lib
    lexer/Lexer.cpp
)

target_link_libraries(chocopc_lib PUBLIC c++abi ${llvm_libs})
target_include_directories(chocopc_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(chocopc chocopc.cpp)

target_link_libraries(chocopc chocopc_lib)