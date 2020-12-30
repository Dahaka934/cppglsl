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

int main() {

    return glsl::test::has_error ? 1 : 0;
}
