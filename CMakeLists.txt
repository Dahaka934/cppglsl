cmake_minimum_required(VERSION 3.17)
project(cppglsl)

set(CMAKE_CXX_STANDARD 20)

add_library(glsl INTERFACE)
target_include_directories(glsl SYSTEM INTERFACE include/)
target_compile_options(glsl INTERFACE -Wall -Wextra -pedantic -Werror -Wconversion)
