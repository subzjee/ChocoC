FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG main
)
FetchContent_MakeAvailable(googletest)

add_executable(
        chocopc_test
        test_token.cpp
        test_lexer.cpp
        test_parser.cpp
)

target_link_libraries(chocopc_test
        PRIVATE GTest::gtest_main c++abi chocopc_lib ${llvm_libs}
)

target_include_directories(chocopc_test PUBLIC ${PROJECT_SOURCE_DIR}/include)

include(GoogleTest)
gtest_discover_tests(chocopc_test)