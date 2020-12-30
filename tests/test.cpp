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

int main() {
    test_vector_default();

    return glsl::test::has_error ? 1 : 0;
}
