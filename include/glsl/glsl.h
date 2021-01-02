#pragma once

#ifndef GLSL_VEC_SWIZZLING
#define GLSL_VEC_SWIZZLING 1
#endif

#ifndef GLSL_VEC_XYZW
#define GLSL_VEC_XYZW 1
#endif

#ifndef GLSL_VEC_RGBA
#define GLSL_VEC_RGBA 1
#endif

#ifndef GLSL_VEC_STPQ
#define GLSL_VEC_STPQ 1
#endif

#include "vector.h"
#include "vector_functions.h"
#include "matrix.h"
#include "matrix_functions.h"

namespace glsl {

using bvec2 = glsl::Vector<bool, 2>;
using bvec3 = glsl::Vector<bool, 3>;
using bvec4 = glsl::Vector<bool, 4>;

using ivec2 = glsl::Vector<int, 2>;
using ivec3 = glsl::Vector<int, 3>;
using ivec4 = glsl::Vector<int, 4>;

using vec2 = glsl::Vector<float, 2>;
using vec3 = glsl::Vector<float, 3>;
using vec4 = glsl::Vector<float, 4>;

using mat2x2 = glsl::Matrix<float, 2, 2>;
using mat2x3 = glsl::Matrix<float, 2, 3>;
using mat2x4 = glsl::Matrix<float, 2, 4>;
using mat3x2 = glsl::Matrix<float, 3, 2>;
using mat3x3 = glsl::Matrix<float, 3, 3>;
using mat3x4 = glsl::Matrix<float, 3, 4>;
using mat4x2 = glsl::Matrix<float, 4, 2>;
using mat4x3 = glsl::Matrix<float, 4, 3>;
using mat4x4 = glsl::Matrix<float, 4, 4>;

using mat2 = mat2x2;
using mat3 = mat3x3;
using mat4 = mat4x4;

} // namespace glsl
