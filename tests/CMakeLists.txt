FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG main
)
FetchContent_MakeAvailable(googletest)

add_executable(
        chocopc_test
        token.cpp
)

target_link_libraries(chocopc_test
        PRIVATE GTest::gtest_main c++abi chocopc_lib
)

include(GoogleTest)
gtest_discover_tests(chocopc_test)