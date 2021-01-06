#pragma once

#include <cmath>
#include <numbers>
#include "details/utils.h"

namespace glsl {

inline constexpr double pi = std::numbers::pi_v<double>;

#define DEF_VEC_FUNC(func, impl)                                    \
template<class... Args>                                             \
constexpr auto func(const Args& ...args) {                          \
    using T = typename traits::first<Args...>::type;                \
    if constexpr (concepts::Vector<T>) {                            \
        constexpr auto fun = func<traits::vector_item_t<Args>...>;  \
        return details::apply(fun, args...);                        \
    } else {                                                        \
        return T(impl(args...));                                    \
    }                                                               \
}                                                                   \

namespace details {

template<concepts::Scalar T>
constexpr T degrees(T x) {
    return x * (180 / std::numbers::pi);
}

template<concepts::Scalar T>
constexpr T radians(T x) {
    return x * (std::numbers::pi / 180);
}

template<concepts::Scalar T>
constexpr T fract(T x) {
    return x - std::floor(x);
}

template<concepts::Scalar T, concepts::Scalar T1 = T>
constexpr T mix(T x, T y, T1 a) {
    if constexpr (std::same_as<T1, bool>) {
        return a ? y : x;
    } else {
        return x + a * (y - x);
    }
}

template<concepts::Scalar T>
constexpr T mod(T x, T y) {
    if constexpr (std::integral<T>) {
        return x % y;
    } else {
        return x - y * std::floor(x / y);
    }
}

template<concepts::Scalar T>
constexpr T sign(T x) {
    return x > 0 ? 1 : x < 0 ? -1 : 0;
}

template<concepts::Scalar T>
constexpr T smoothstep(T edge0, T edge1, T x) {
    T t = std::clamp((x - edge0) / (edge1 - edge0), T(0), T(1));
    return t * t * (T(3) - T(2) * t);
}

template<concepts::Scalar T>
constexpr T inversesqrt(T x) {
    return T(1) / std::sqrt(x);
}

template<concepts::Scalar T>
constexpr T step(T edge, T x) {
    return x < edge ? T(0) : T(1);
}

template<concepts::Scalar T>
constexpr T faceforward(T n, T i, T nref) {
    return (nref * i) < 0 ? T(n) : T(-n);
}

template<concepts::Scalar T>
constexpr T length(T x) {
    return x ? x : -x;
}

template<concepts::Scalar T>
constexpr T equal(T x, T y) {
    return x == y;
}

template<concepts::Scalar T>
constexpr T notEqual(T x, T y) {
    return x != y;
}

template<concepts::Scalar T>
constexpr T greaterThan(T x, T y) {
    return x > y;
}

template<concepts::Scalar T>
constexpr T greaterThanEqual(T x, T y) {
    return x >= y;
}

template<concepts::Scalar T>
constexpr T lessThan(T x, T y) {
    return x < y;
}

template<concepts::Scalar T>
constexpr T lessThanEqual(T x, T y) {
    return x <= y;
}

} // namespace details

DEF_VEC_FUNC(acos, std::acos)
DEF_VEC_FUNC(asin, std::asin)
DEF_VEC_FUNC(cos, std::cos)
DEF_VEC_FUNC(sin, std::sin)
DEF_VEC_FUNC(tan, std::tan)
DEF_VEC_FUNC(degrees, details::degrees)
DEF_VEC_FUNC(radians, details::radians)
DEF_VEC_FUNC(abs, std::abs)
DEF_VEC_FUNC(ceil, std::ceil)
DEF_VEC_FUNC(exp, std::exp)
DEF_VEC_FUNC(exp2, std::exp2)
DEF_VEC_FUNC(floor, std::floor)
DEF_VEC_FUNC(fract, details::fract)
DEF_VEC_FUNC(isinf, std::isinf)
DEF_VEC_FUNC(isnan, std::isnan)
DEF_VEC_FUNC(log, std::log)
DEF_VEC_FUNC(log2, std::log2)
DEF_VEC_FUNC(max, std::max)
DEF_VEC_FUNC(min, std::min)
DEF_VEC_FUNC(clamp, std::clamp)
DEF_VEC_FUNC(mod, details::mod)
DEF_VEC_FUNC(pow, std::pow)
DEF_VEC_FUNC(round, std::round)
DEF_VEC_FUNC(sign, details::sign)
DEF_VEC_FUNC(smoothstep, details::smoothstep)
DEF_VEC_FUNC(sqrt, std::sqrt)
DEF_VEC_FUNC(inversesqrt, details::inversesqrt)
DEF_VEC_FUNC(step, details::step)
DEF_VEC_FUNC(trunc, std::trunc)
DEF_VEC_FUNC(faceforward, details::faceforward)
DEF_VEC_FUNC(mix, details::mix)

template<concepts::Vector T>
constexpr auto mix(const T& x, const T& y) {
    constexpr auto rx = x[1] * y[2] - x[2] * y[1];
    constexpr auto ry = x[2] * y[0] - x[0] * y[2];
    constexpr auto rz = x[0] * y[1] - x[1] * y[0];
    return T(rx, ry, rz);
}

template<class T>
constexpr auto dot(const T& x, const T& y) {
    if constexpr (concepts::Vector<T>) {
        decltype(dot(x[0], y[0])) result(0);
        details::vector_foreach<T>([&](size_t index) {
            result += dot(x[index], y[index]);
        });
        return result;
    } else {
        return x * y;
    }
}

template<class T>
constexpr auto length(const T& x) {
    return glsl::sqrt(glsl::dot(x, x));
}

template<class T>
constexpr auto distance(const T& x, const T& y) {
    return glsl::length(x - y);
}

template<class T>
constexpr auto normalize(const T& x) {
    return x * glsl::inversesqrt(glsl::dot(x, x));
}

template<class T>
constexpr auto reflect(const T& x, const T& n) {
    return x - 2.0 * glsl::dot(n, x) * n;
}

template<class T>
constexpr bool all(const T& x) {
    if constexpr (concepts::Vector<T>) {
        bool result = true;
        details::vector_foreach<T>([&](size_t index) {
            result &= all(x[index]);
        });
        return result;
    } else {
        return bool(x);
    }
}

template<class T>
constexpr bool any(const T& x) {
    if constexpr (concepts::Vector<T>) {
        bool result = false;
        details::vector_foreach<T>([&](size_t index) {
            result |= all(x[index]);
        });
        return result;
    } else {
        return bool(x);
    }
}

DEF_VEC_FUNC(equal, details::equal)
DEF_VEC_FUNC(notEqual, details::notEqual)
DEF_VEC_FUNC(greaterThan, details::greaterThan)
DEF_VEC_FUNC(greaterThanEqual, details::greaterThanEqual)
DEF_VEC_FUNC(lessThan, details::lessThan)
DEF_VEC_FUNC(lessThanEqual, details::lessThanEqual)

#undef DEF_VEC_FUNC

} // namespace glsl
