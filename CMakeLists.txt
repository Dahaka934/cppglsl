cmake_minimum_required(VERSION 3.17)
project(cppglsl)

set(CMAKE_CXX_STANDARD 20)

add_library(glsl INTERFACE)
target_include_directories(glsl SYSTEM INTERFACE include/)
target_compile_options(glsl INTERFACE -Wall -Wextra -pedantic -Werror -Wconversion)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif()
