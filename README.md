# cppglsl
Modern (c++20) header-only library bringing glsl shader language syntax into C++.

Features:
* Implemented generic Vector and Matrix classes.
* Implemented vector swizzling.
* Almost all glsl functions are implemented for working with vectors and matrices.
* Full constexpr (except swizzling).
* Use fold expressions and concepts.

Examples:

      vec4 v(1);                  // (1,1,1,1)
      v.x = 2;                    // (2,1,1,1)
      v.zy = vec2(4, 3);          // (2,3,4,1)
      v.xw = v.yz * 2;            // (6,3,4,8)
      vec2 q = max(v.xy, v.zw);   // (6,8)
