#include "test.h"

using namespace glsl;
using namespace glsl::test;

#define CHECK(expr, expected) check(expr, expected, #expr)
#define CHECK_BLOCK(block, expected) CHECK([]block(), expected)

static_assert(std::constructible_from<vec3, float>);
static_assert(std::constructible_from<vec3, float, float> == false);
static_assert(std::constructible_from<vec3, float, float, float>);
static_assert(std::constructible_from<vec3, float, float, float, float> == false);
static_assert(std::constructible_from<vec3, int>);
static_assert(std::constructible_from<vec3, int, int> == false);
static_assert(std::constructible_from<vec3, int, int, int>);
static_assert(std::constructible_from<vec3, int, int, int, int> == false);
static_assert(std::constructible_from<vec3, vec2> == false);
static_assert(std::constructible_from<vec3, vec2, float>);
static_assert(std::constructible_from<vec3, float, vec2>);
static_assert(std::constructible_from<vec3, vec3>);
static_assert(std::constructible_from<vec4, float, int, double, bool>);
static_assert(std::constructible_from<vec4, vec2, float, vec2> == false);
static_assert(std::constructible_from<vec4, vec3, float>);
static_assert(std::constructible_from<vec4, float, vec2, int>);

void test_vector_default() {
    CHECK(vec3(1), vec3(1, 1, 1));
    CHECK(vec3(1, 2, 3).zzz, vec3(3));
    CHECK(vec3(1, 2, 3).zxyz, vec4(3, 1, 2, 3));
    CHECK(vec3x3(1, 2, 3), vec3x3(vec3(1), vec3(2), vec3(3)));
    CHECK(vec3x3(vec3(1, 2, 3)), vec3x3(vec3(1, 2, 3), vec3(1, 2, 3), vec3(1, 2, 3)));
    CHECK(vec3x3(vec3(1), vec3(1, 2, 3), vec3(5)), vec3x3(vec3(1, 1, 1), vec3(1, 2, 3), vec3(5, 5, 5)));

    CHECK(+vec3(1), vec3(+1, +1, +1));
    CHECK(-vec3(1), vec3(-1, -1, -1));
    CHECK(++vec3(1, 2, 3), vec3(2, 3, 4));

    CHECK_BLOCK({
        vec3 v(1, 2, 3);
        v = ivec3(5, 6, 7);
        return v;
    }, vec3(5, 6, 7));

    CHECK(!vec3(0, 4, 1), ivec3(1, 0, 0));

    CHECK(vec3(1, 2, 3) + vec3(1), vec3(2, 3, 4));
    CHECK(vec3(1, 2, 3) + 1, vec3(2, 3, 4));
    CHECK(1 + vec3(1, 2, 3), vec3(2, 3, 4));

    CHECK(vec3x3(1, 2, 3) + vec3x3(1), vec3x3(2, 3, 4));
    CHECK(vec3x3(1, 2, 3) + vec3(1), vec3x3(2, 3, 4));
    CHECK(vec3(1) + vec3x3(1, 2, 3), vec3x3(2, 3, 4));
    CHECK(vec3x3(1, 2, 3) + 1, vec3x3(2, 3, 4));
    CHECK(1 + vec3x3(1, 2, 3), vec3x3(2, 3, 4));

    CHECK_BLOCK({
        ivec3 v(1, 2, 3);
        return std::accumulate(v.begin(), v.end(), 0);
    }, 6);
}

void test_vector_functions() {
    CHECK(cos(0.0), 1.0);
    CHECK(cos(pi), -1.0);
    CHECK(cos(vec2(pi, pi*2)), vec2(-1.0, 1.0));
    CHECK(cos(vec4x2(vec2(pi, pi*2))), vec2(-1.0, 1.0));
    CHECK(radians(degrees(pi)), pi);
    CHECK(degrees(radians(vec3(1, 2, 3))), vec3(1, 2, 3));

    CHECK(abs(vec2(-1, 1)), vec2(1, 1));
    CHECK(isinf(vec2(1)), bvec2(0, 0));

    CHECK(max(vec2(1, 2), vec2(3, 0)), vec2(3, 2));
    CHECK(clamp(vec3(0, 5, 9), vec3(3), vec3(6)), vec3(3, 5, 6));
    CHECK(mix(vec2(0), vec2(5), bvec2(1, 0)), vec2(5, 0));
    CHECK(mix(vec2(0), vec2(4), vec2(0.5, 2)), vec2(2, 8));
    CHECK(sign(vec3(-100, 0, 100)), ivec3(-1, 0, 1));

    CHECK(dot(5, 10), 50);
    CHECK(dot(ivec2(5), ivec2(10)), 100);

    CHECK(length(-5), 5);
    CHECK(length(ivec2(3, 4).xy), 5);
}

void test_matrix() {
    CHECK(mat3(1)[0], vec3(1, 0, 0));
    CHECK(mat3(1)[2], vec3(0, 0, 1));
    CHECK(mat2(1, 2, 3, 4)[1], vec2(3, 4));
    CHECK(mat2(1, vec2(2, 3), 4)[1], vec2(3, 4));
    CHECK(mat2(1, 2, 3, 4).row(1), vec2(2, 4));
    CHECK(-mat2(1), mat2(-1));
    CHECK(mat2(1) + mat2(1), mat2(2, 0, 0, 2));
    CHECK(mat2(1) + 1, mat2(2, 1, 1, 2));
    CHECK(1 + mat2(1), mat2(2, 1, 1, 2));
}

int main() {
    test_vector_default();
    test_vector_functions();
    test_matrix();

    return glsl::test::has_error ? 1 : 0;
}
