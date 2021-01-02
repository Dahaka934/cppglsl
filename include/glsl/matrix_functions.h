#pragma once

#include <cmath>
#include "matrix.h"

namespace glsl {

template<concepts::MatrixQuad T>
constexpr auto determinant(const T& m) {
    constexpr size_t M = T::MatrixColumns;

    if constexpr (M == 1) {
        return m[0][0];
    } else if constexpr (M == 2) {
        return m[0][0] * m[1][1] - m[1][0] * m[0][1];
    } else if constexpr (M == 3) {
        return m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] -
               m[0][1] * m[1][0] * m[2][2] + m[0][1] * m[1][2] * m[2][0] +
               m[0][2] * m[1][0] * m[2][1] - m[0][2] * m[1][1] * m[2][0];
    } else {
        typename T::MatrixItem det(1);
        T temp(m);

        for (size_t c = 0; c < M; ++c) {
            det = det * temp[c][c];
            for (size_t r = c + 1; r < M; ++r) {
                auto ratio = temp[c][r] / temp[c][c];
                for (size_t k = c; k < M; k++) {
                    temp[k][r] = temp[k][r] - ratio * temp[k][c];
                }
            }
        }

        return det;
    }
}

template<concepts::Matrix T>
constexpr auto transpose(const T& m) {
    constexpr size_t M = T::MatrixColumns;
    constexpr size_t N = T::MatrixRows;

    if constexpr (M == 1 && N == 1) {
        return m;
    } else {
        T result;
        details::static_foreach<0, N>([&](auto i) {
            result[i] = m.row(i);
        });
        return result;
    }
}

template<concepts::MatrixQuadN<1> T>
constexpr auto inverse(const T& m) {
    return T(1 / determinant(m));
}

template<concepts::MatrixQuadN<2> T>
constexpr auto inverse(const T& m) {
    return T(m[1][1], -m[0][1], -m[1][0], m[0][0]) / determinant(m);
}

template<concepts::MatrixQuadN<3> T>
constexpr auto inverse(const T& m) {
    auto a00 = m[0][0], a01 = m[0][1], a02 = m[0][2];
    auto a10 = m[1][0], a11 = m[1][1], a12 = m[1][2];
    auto a20 = m[2][0], a21 = m[2][1], a22 = m[2][2];

    auto b01 = a22 * a11 - a12 * a21;
    auto b11 = a12 * a20 - a22 * a10;
    auto b21 = a21 * a10 - a11 * a20;

    auto det = a00 * b01 + a01 * b11 + a02 * b21;

    return T(b01, (a02 * a21 - a22 * a01), (a12 * a01 - a02 * a11),
             b11, (a22 * a00 - a02 * a20), (a02 * a10 - a12 * a00),
             b21, (a01 * a20 - a21 * a00), (a11 * a00 - a01 * a10)) / det;
}

template<concepts::MatrixQuadN<4> T>
constexpr auto inverse(const T& m) {
    auto a00 = m[0][0], a01 = m[0][1], a02 = m[0][2], a03 = m[0][3];
    auto a10 = m[1][0], a11 = m[1][1], a12 = m[1][2], a13 = m[1][3];
    auto a20 = m[2][0], a21 = m[2][1], a22 = m[2][2], a23 = m[2][3];
    auto a30 = m[3][0], a31 = m[3][1], a32 = m[3][2], a33 = m[3][3];

    auto b00 = a00 * a11 - a01 * a10;
    auto b01 = a00 * a12 - a02 * a10;
    auto b02 = a00 * a13 - a03 * a10;
    auto b03 = a01 * a12 - a02 * a11;
    auto b04 = a01 * a13 - a03 * a11;
    auto b05 = a02 * a13 - a03 * a12;
    auto b06 = a20 * a31 - a21 * a30;
    auto b07 = a20 * a32 - a22 * a30;
    auto b08 = a20 * a33 - a23 * a30;
    auto b09 = a21 * a32 - a22 * a31;
    auto b10 = a21 * a33 - a23 * a31;
    auto b11 = a22 * a33 - a23 * a32;

    auto det = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;

    return T(a11 * b11 - a12 * b10 + a13 * b09,
             a02 * b10 - a01 * b11 - a03 * b09,
             a31 * b05 - a32 * b04 + a33 * b03,
             a22 * b04 - a21 * b05 - a23 * b03,
             a12 * b08 - a10 * b11 - a13 * b07,
             a00 * b11 - a02 * b08 + a03 * b07,
             a32 * b02 - a30 * b05 - a33 * b01,
             a20 * b05 - a22 * b02 + a23 * b01,
             a10 * b10 - a11 * b08 + a13 * b06,
             a01 * b08 - a00 * b10 - a03 * b06,
             a30 * b04 - a31 * b02 + a33 * b00,
             a21 * b02 - a20 * b04 - a23 * b00,
             a11 * b07 - a10 * b09 - a12 * b06,
             a00 * b09 - a01 * b07 + a02 * b06,
             a31 * b01 - a30 * b03 - a32 * b00,
             a20 * b03 - a21 * b01 + a22 * b00) / det;
}

} // namespace glsl
