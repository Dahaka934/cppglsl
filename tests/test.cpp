#include "test.h"

#define CHECK(expr, expected) check(expr, expected, #expr)
#define CHECK_BLOCK(block, expected) CHECK([]block(), expected)

int main() {

    return glsl::test::has_error ? 1 : 0;
}
