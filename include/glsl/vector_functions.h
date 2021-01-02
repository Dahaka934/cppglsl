#pragma once

#include <cmath>
#include <numbers>
#include "details/utils.h"

namespace glsl {

inline constexpr double pi = std::numbers::pi_v<double>;

#define NARGS(...) \
    NARGS_(0, ##__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define NARGS_(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N

#define GLUE(x, y) GLUE_(x, y)
#define GLUE_(x, y) x##y

#define FUN_ARGS(type, ...) GLUE(FUN_ARGS, NARGS(__VA_ARGS__))(type, __VA_ARGS__)
#define FUN_ARGS1(type, x) type x
#define FUN_ARGS2(type, x, ...) FUN_ARGS1(type, x), FUN_ARGS1(type, __VA_ARGS__)
#define FUN_ARGS3(type, x, ...) FUN_ARGS1(type, x), FUN_ARGS2(type, __VA_ARGS__)

#define DEF_VEC_FUNC(func, impl, ...)                           \
template<class T>                                               \
constexpr auto func(FUN_ARGS(const T&, __VA_ARGS__)) {          \
    if constexpr (concepts::Vector<T>) {                        \
        constexpr auto fun = func<traits::vector_item_t<T>>;    \
        return details::apply(fun, __VA_ARGS__);                \
    } else {                                                    \
        return T(impl(__VA_ARGS__));                            \
    }                                                           \
}                                                               \

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

DEF_VEC_FUNC(acos, std::acos, rad)
DEF_VEC_FUNC(asin, std::asin, rad)
DEF_VEC_FUNC(cos, std::cos, rad)
DEF_VEC_FUNC(sin, std::sin, rad)
DEF_VEC_FUNC(tan, std::tan, rad)
DEF_VEC_FUNC(degrees, details::degrees, rad)
DEF_VEC_FUNC(radians, details::radians, deg)
DEF_VEC_FUNC(abs, std::abs, x)
DEF_VEC_FUNC(ceil, std::ceil, x)
DEF_VEC_FUNC(exp, std::exp, x)
DEF_VEC_FUNC(exp2, std::exp2, x)
DEF_VEC_FUNC(floor, std::floor, x)
DEF_VEC_FUNC(fract, details::fract, x)
DEF_VEC_FUNC(isinf, std::isinf, x)
DEF_VEC_FUNC(isnan, std::isnan, x)
DEF_VEC_FUNC(log, std::log, x)
DEF_VEC_FUNC(log2, std::log2, x)
DEF_VEC_FUNC(max, std::max, x, y)
DEF_VEC_FUNC(min, std::min, x, y)
DEF_VEC_FUNC(clamp, std::clamp, x, min, max)
DEF_VEC_FUNC(mod, details::mod, x, y)
DEF_VEC_FUNC(pow, std::pow, x, y)
DEF_VEC_FUNC(round, std::round, x)
DEF_VEC_FUNC(sign, details::sign, x)
DEF_VEC_FUNC(smoothstep, details::smoothstep, edge0, edge1, x)
DEF_VEC_FUNC(sqrt, std::sqrt, x)
DEF_VEC_FUNC(inversesqrt, details::inversesqrt, x)
DEF_VEC_FUNC(step, details::step, edge, x)
DEF_VEC_FUNC(trunc, std::trunc, x)
DEF_VEC_FUNC(faceforward, details::faceforward, n, i, nref)

template<class T, class B>
constexpr auto mix(const T& x, const T& y, const B& a) {
    if constexpr (concepts::Vector<T>) {
        constexpr auto fun = mix<traits::vector_item_t<T>, traits::vector_item_t<B>>;
        return details::apply(fun, x, y, a);
    } else {
        return details::mix(x, y, a);
    }
}

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

DEF_VEC_FUNC(equal, details::equal, x, y)
DEF_VEC_FUNC(notEqual, details::notEqual, x, y)
DEF_VEC_FUNC(greaterThan, details::greaterThan, x, y)
DEF_VEC_FUNC(greaterThanEqual, details::greaterThanEqual, x, y)
DEF_VEC_FUNC(lessThan, details::lessThan, x, y)
DEF_VEC_FUNC(lessThanEqual, details::lessThanEqual, x, y)

#undef NARGS
#undef NARGS_
#undef GLUE
#undef GLUE_
#undef FUN_ARGS
#undef FUN_ARGS1
#undef FUN_ARGS2
#undef FUN_ARGS3
#undef DEF_VEC_FUNC

} // namespace glsl
