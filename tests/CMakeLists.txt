FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG main
)
FetchContent_MakeAvailable(googletest)

add_executable(
        test_chocopc
        test_lexer.cpp
        test_parser.cpp
)

target_link_libraries(test_chocopc
        PRIVATE GTest::gtest_main libchoco
)

target_include_directories(test_chocopc PUBLIC ${PROJECT_SOURCE_DIR}/include)

// Prevent Google Test from including `cxxabi.h` from LLVM's include directory.
target_compile_definitions(test_chocopc PRIVATE GTEST_HAS_CXXABI_H_=0)

include(GoogleTest)
gtest_discover_tests(test_chocopc)
