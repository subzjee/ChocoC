cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -fmodules -fbuiltin-module-map -Wall -Wextra -pedantic")

project(chocopc
        LANGUAGES C CXX)

include(FetchContent)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)