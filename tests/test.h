#pragma once

#include <iostream>
#include <numeric>
#include <ranges>

#include "glsl/glsl.h"

namespace glsl::test {

static bool has_error = false;

template<class T>
constexpr auto to_string(const T &obj) {
    return obj;
}

template<>
auto to_string<bool>(const bool &obj) {
    return std::string(obj ? "true" : "false");
}

template<class... Args>
constexpr void println(const Args &... args) {
    (std::cout << ... << to_string(args)) << std::endl;
}

template<class... Args>
constexpr void error(const Args &... args) {
    (std::cout << ... << to_string(args)) << std::endl;
}

template<class T, class TE>
constexpr void check(T &&result, TE &&expected, std::string_view msg = "")
requires requires { result != expected; } {
    println("check: ", msg, " -> ", result);
    if (result != expected) {
        error("    expected: ", expected);
        has_error = true;
    }
}

} // namespace glsl::test
