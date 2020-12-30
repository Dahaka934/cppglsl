#pragma once

#include "vector_proxy.h"

namespace glsl {

#define DEF_BASE(Type)                                                              \
    constexpr Type() : data{}{}                                                     \
    constexpr Type(const Type&) = default;                                          \
    constexpr Type(Type&&) noexcept = default;                                      \
    constexpr Type& operator=(const Type&) = default;                               \
    constexpr Type& operator=(Type&&) noexcept = default;                           \

#define VEC_INDEX_x 0
#define VEC_INDEX_y 1
#define VEC_INDEX_z 2
#define VEC_INDEX_w 3
#define VEC_INDEX_r 0
#define VEC_INDEX_g 1
#define VEC_INDEX_b 2
#define VEC_INDEX_a 3
#define VEC_INDEX_s 0
#define VEC_INDEX_t 1
#define VEC_INDEX_p 2
#define VEC_INDEX_q 3
#define VEC_INDEX(v)                    (VEC_INDEX_##v)
#define VEC_PROXY                       typename TraitType::template Proxy
#define VEC_SWIZZLING1(v1)              VEC_PROXY<VEC_INDEX(v1)> v1
#define VEC_SWIZZLING2(v1, v2)          VEC_PROXY<VEC_INDEX(v1), VEC_INDEX(v2)> v1##v2
#define VEC_SWIZZLING3(v1, v2, v3)      VEC_PROXY<VEC_INDEX(v1), VEC_INDEX(v2), VEC_INDEX(v3)> v1##v2##v3
#define VEC_SWIZZLING4(v1, v2, v3, v4)  VEC_PROXY<VEC_INDEX(v1), VEC_INDEX(v2), VEC_INDEX(v3), VEC_INDEX(v4)> v1##v2##v3##v4

template<class Scalar, size_t Size, template<class, size_t> class Trait>
struct VectorBase {
    using TraitType = Trait<Scalar, Size>;

    DEF_BASE(VectorBase)

    typename TraitType::DataType data;
};

#if GLSL_VEC_SWIZZLING

template<class Scalar, template<class, size_t> class Trait>
struct VectorBase<Scalar, 1, Trait> {
    using TraitType = Trait<Scalar, 1>;

    DEF_BASE(VectorBase)

    union {
        typename TraitType::DataType data;

#define DEF_VEC_FIELDS(x)               \
        struct {                        \
            VEC_SWIZZLING1(x);          \
        };                              \
                                        \
        VEC_SWIZZLING2(x, x);           \
        VEC_SWIZZLING3(x, x, x);        \
        VEC_SWIZZLING4(x, x, x, x);     \

#if GLSL_VEC_XYZW == 1
        DEF_VEC_FIELDS(x)
#endif
#if GLSL_VEC_RGBA == 1
        DEF_VEC_FIELDS(r)
#endif
#if GLSL_VEC_STPQ == 1
        DEF_VEC_FIELDS(s)
#endif
#undef DEF_VEC_FIELDS
    };
};

template<class Scalar, template<class, size_t> class Trait>
struct VectorBase<Scalar, 2, Trait> {
    using TraitType = Trait<Scalar, 2>;

    DEF_BASE(VectorBase)

    union {
        typename TraitType::DataType data;

#define DEF_VEC_FIELDS(x, y)            \
        struct {                        \
            VEC_SWIZZLING1(x);          \
            VEC_SWIZZLING1(y);          \
        };                              \
                                        \
        VEC_SWIZZLING2(x, x);           \
        VEC_SWIZZLING2(x, y);           \
        VEC_SWIZZLING2(y, x);           \
        VEC_SWIZZLING2(y, y);           \
        VEC_SWIZZLING3(x, x, x);        \
        VEC_SWIZZLING3(x, x, y);        \
        VEC_SWIZZLING3(x, y, x);        \
        VEC_SWIZZLING3(x, y, y);        \
        VEC_SWIZZLING3(y, x, x);        \
        VEC_SWIZZLING3(y, x, y);        \
        VEC_SWIZZLING3(y, y, x);        \
        VEC_SWIZZLING3(y, y, y);        \
        VEC_SWIZZLING4(x, x, x, x);     \
        VEC_SWIZZLING4(x, x, x, y);     \
        VEC_SWIZZLING4(x, x, y, x);     \
        VEC_SWIZZLING4(x, x, y, y);     \
        VEC_SWIZZLING4(x, y, x, x);     \
        VEC_SWIZZLING4(x, y, x, y);     \
        VEC_SWIZZLING4(x, y, y, x);     \
        VEC_SWIZZLING4(x, y, y, y);     \
        VEC_SWIZZLING4(y, x, x, x);     \
        VEC_SWIZZLING4(y, x, x, y);     \
        VEC_SWIZZLING4(y, x, y, x);     \
        VEC_SWIZZLING4(y, x, y, y);     \
        VEC_SWIZZLING4(y, y, x, x);     \
        VEC_SWIZZLING4(y, y, x, y);     \
        VEC_SWIZZLING4(y, y, y, x);     \
        VEC_SWIZZLING4(y, y, y, y);     \

#if GLSL_VEC_XYZW == 1
        DEF_VEC_FIELDS(x, y)
#endif
#if GLSL_VEC_RGBA == 1
        DEF_VEC_FIELDS(r, g)
#endif
#if GLSL_VEC_STPQ == 1
        DEF_VEC_FIELDS(s, t)
#endif
#undef DEF_VEC_FIELDS
    };
};

template<class Scalar, template<class, size_t> class Trait>
struct VectorBase<Scalar, 3, Trait> {
    using TraitType = Trait<Scalar, 3>;

    DEF_BASE(VectorBase)

    union {
        typename TraitType::DataType data;

#define DEF_VEC_FIELDS(x, y, z)         \
        struct {                        \
            VEC_SWIZZLING1(x);          \
            VEC_SWIZZLING1(y);          \
            VEC_SWIZZLING1(z);          \
        };                              \
                                        \
        VEC_SWIZZLING2(x, x);           \
        VEC_SWIZZLING2(x, y);           \
        VEC_SWIZZLING2(x, z);           \
        VEC_SWIZZLING2(y, x);           \
        VEC_SWIZZLING2(y, y);           \
        VEC_SWIZZLING2(y, z);           \
        VEC_SWIZZLING2(z, x);           \
        VEC_SWIZZLING2(z, y);           \
        VEC_SWIZZLING2(z, z);           \
        VEC_SWIZZLING3(x, x, x);        \
        VEC_SWIZZLING3(x, x, y);        \
        VEC_SWIZZLING3(x, x, z);        \
        VEC_SWIZZLING3(x, y, x);        \
        VEC_SWIZZLING3(x, y, y);        \
        VEC_SWIZZLING3(x, y, z);        \
        VEC_SWIZZLING3(x, z, x);        \
        VEC_SWIZZLING3(x, z, y);        \
        VEC_SWIZZLING3(x, z, z);        \
        VEC_SWIZZLING3(y, x, x);        \
        VEC_SWIZZLING3(y, x, y);        \
        VEC_SWIZZLING3(y, x, z);        \
        VEC_SWIZZLING3(y, y, x);        \
        VEC_SWIZZLING3(y, y, y);        \
        VEC_SWIZZLING3(y, y, z);        \
        VEC_SWIZZLING3(y, z, x);        \
        VEC_SWIZZLING3(y, z, y);        \
        VEC_SWIZZLING3(y, z, z);        \
        VEC_SWIZZLING3(z, x, x);        \
        VEC_SWIZZLING3(z, x, y);        \
        VEC_SWIZZLING3(z, x, z);        \
        VEC_SWIZZLING3(z, y, x);        \
        VEC_SWIZZLING3(z, y, y);        \
        VEC_SWIZZLING3(z, y, z);        \
        VEC_SWIZZLING3(z, z, x);        \
        VEC_SWIZZLING3(z, z, y);        \
        VEC_SWIZZLING3(z, z, z);        \
        VEC_SWIZZLING4(x, x, x, x);     \
        VEC_SWIZZLING4(x, x, x, y);     \
        VEC_SWIZZLING4(x, x, x, z);     \
        VEC_SWIZZLING4(x, x, y, x);     \
        VEC_SWIZZLING4(x, x, y, y);     \
        VEC_SWIZZLING4(x, x, y, z);     \
        VEC_SWIZZLING4(x, x, z, x);     \
        VEC_SWIZZLING4(x, x, z, y);     \
        VEC_SWIZZLING4(x, x, z, z);     \
        VEC_SWIZZLING4(x, y, x, x);     \
        VEC_SWIZZLING4(x, y, x, y);     \
        VEC_SWIZZLING4(x, y, x, z);     \
        VEC_SWIZZLING4(x, y, y, x);     \
        VEC_SWIZZLING4(x, y, y, y);     \
        VEC_SWIZZLING4(x, y, y, z);     \
        VEC_SWIZZLING4(x, y, z, x);     \
        VEC_SWIZZLING4(x, y, z, y);     \
        VEC_SWIZZLING4(x, y, z, z);     \
        VEC_SWIZZLING4(x, z, x, x);     \
        VEC_SWIZZLING4(x, z, x, y);     \
        VEC_SWIZZLING4(x, z, x, z);     \
        VEC_SWIZZLING4(x, z, y, x);     \
        VEC_SWIZZLING4(x, z, y, y);     \
        VEC_SWIZZLING4(x, z, y, z);     \
        VEC_SWIZZLING4(x, z, z, x);     \
        VEC_SWIZZLING4(x, z, z, y);     \
        VEC_SWIZZLING4(x, z, z, z);     \
        VEC_SWIZZLING4(y, x, x, x);     \
        VEC_SWIZZLING4(y, x, x, y);     \
        VEC_SWIZZLING4(y, x, x, z);     \
        VEC_SWIZZLING4(y, x, y, x);     \
        VEC_SWIZZLING4(y, x, y, y);     \
        VEC_SWIZZLING4(y, x, y, z);     \
        VEC_SWIZZLING4(y, x, z, x);     \
        VEC_SWIZZLING4(y, x, z, y);     \
        VEC_SWIZZLING4(y, x, z, z);     \
        VEC_SWIZZLING4(y, y, x, x);     \
        VEC_SWIZZLING4(y, y, x, y);     \
        VEC_SWIZZLING4(y, y, x, z);     \
        VEC_SWIZZLING4(y, y, y, x);     \
        VEC_SWIZZLING4(y, y, y, y);     \
        VEC_SWIZZLING4(y, y, y, z);     \
        VEC_SWIZZLING4(y, y, z, x);     \
        VEC_SWIZZLING4(y, y, z, y);     \
        VEC_SWIZZLING4(y, y, z, z);     \
        VEC_SWIZZLING4(y, z, x, x);     \
        VEC_SWIZZLING4(y, z, x, y);     \
        VEC_SWIZZLING4(y, z, x, z);     \
        VEC_SWIZZLING4(y, z, y, x);     \
        VEC_SWIZZLING4(y, z, y, y);     \
        VEC_SWIZZLING4(y, z, y, z);     \
        VEC_SWIZZLING4(y, z, z, x);     \
        VEC_SWIZZLING4(y, z, z, y);     \
        VEC_SWIZZLING4(y, z, z, z);     \
        VEC_SWIZZLING4(z, x, x, x);     \
        VEC_SWIZZLING4(z, x, x, y);     \
        VEC_SWIZZLING4(z, x, x, z);     \
        VEC_SWIZZLING4(z, x, y, x);     \
        VEC_SWIZZLING4(z, x, y, y);     \
        VEC_SWIZZLING4(z, x, y, z);     \
        VEC_SWIZZLING4(z, x, z, x);     \
        VEC_SWIZZLING4(z, x, z, y);     \
        VEC_SWIZZLING4(z, x, z, z);     \
        VEC_SWIZZLING4(z, y, x, x);     \
        VEC_SWIZZLING4(z, y, x, y);     \
        VEC_SWIZZLING4(z, y, x, z);     \
        VEC_SWIZZLING4(z, y, y, x);     \
        VEC_SWIZZLING4(z, y, y, y);     \
        VEC_SWIZZLING4(z, y, y, z);     \
        VEC_SWIZZLING4(z, y, z, x);     \
        VEC_SWIZZLING4(z, y, z, y);     \
        VEC_SWIZZLING4(z, y, z, z);     \
        VEC_SWIZZLING4(z, z, x, x);     \
        VEC_SWIZZLING4(z, z, x, y);     \
        VEC_SWIZZLING4(z, z, x, z);     \
        VEC_SWIZZLING4(z, z, y, x);     \
        VEC_SWIZZLING4(z, z, y, y);     \
        VEC_SWIZZLING4(z, z, y, z);     \
        VEC_SWIZZLING4(z, z, z, x);     \
        VEC_SWIZZLING4(z, z, z, y);     \
        VEC_SWIZZLING4(z, z, z, z);     \

#if GLSL_VEC_XYZW == 1
        DEF_VEC_FIELDS(x, y, z)
#endif
#if GLSL_VEC_RGBA == 1
        DEF_VEC_FIELDS(r, g, b)
#endif
#if GLSL_VEC_STPQ == 1
        DEF_VEC_FIELDS(s, t, p)
#endif
#undef DEF_VEC_FIELDS
    };
};

template<class Scalar, template<class, size_t> class Trait>
struct VectorBase<Scalar, 4, Trait> {
    using TraitType = Trait<Scalar, 4>;

    DEF_BASE(VectorBase)

    union {
        typename TraitType::DataType data;

#define DEF_VEC_FIELDS(x, y, z, w)      \
        struct {                        \
            VEC_SWIZZLING1(x);          \
            VEC_SWIZZLING1(y);          \
            VEC_SWIZZLING1(z);          \
            VEC_SWIZZLING1(w);          \
        };                              \
                                        \
        VEC_SWIZZLING2(x, x);           \
        VEC_SWIZZLING2(x, y);           \
        VEC_SWIZZLING2(x, z);           \
        VEC_SWIZZLING2(x, w);           \
        VEC_SWIZZLING2(y, x);           \
        VEC_SWIZZLING2(y, y);           \
        VEC_SWIZZLING2(y, z);           \
        VEC_SWIZZLING2(y, w);           \
        VEC_SWIZZLING2(z, x);           \
        VEC_SWIZZLING2(z, y);           \
        VEC_SWIZZLING2(z, z);           \
        VEC_SWIZZLING2(z, w);           \
        VEC_SWIZZLING2(w, x);           \
        VEC_SWIZZLING2(w, y);           \
        VEC_SWIZZLING2(w, z);           \
        VEC_SWIZZLING2(w, w);           \
        VEC_SWIZZLING3(x, x, x);        \
        VEC_SWIZZLING3(x, x, y);        \
        VEC_SWIZZLING3(x, x, z);        \
        VEC_SWIZZLING3(x, x, w);        \
        VEC_SWIZZLING3(x, y, x);        \
        VEC_SWIZZLING3(x, y, y);        \
        VEC_SWIZZLING3(x, y, z);        \
        VEC_SWIZZLING3(x, y, w);        \
        VEC_SWIZZLING3(x, z, x);        \
        VEC_SWIZZLING3(x, z, y);        \
        VEC_SWIZZLING3(x, z, z);        \
        VEC_SWIZZLING3(x, z, w);        \
        VEC_SWIZZLING3(x, w, x);        \
        VEC_SWIZZLING3(x, w, y);        \
        VEC_SWIZZLING3(x, w, z);        \
        VEC_SWIZZLING3(x, w, w);        \
        VEC_SWIZZLING3(y, x, x);        \
        VEC_SWIZZLING3(y, x, y);        \
        VEC_SWIZZLING3(y, x, z);        \
        VEC_SWIZZLING3(y, x, w);        \
        VEC_SWIZZLING3(y, y, x);        \
        VEC_SWIZZLING3(y, y, y);        \
        VEC_SWIZZLING3(y, y, z);        \
        VEC_SWIZZLING3(y, y, w);        \
        VEC_SWIZZLING3(y, z, x);        \
        VEC_SWIZZLING3(y, z, y);        \
        VEC_SWIZZLING3(y, z, z);        \
        VEC_SWIZZLING3(y, z, w);        \
        VEC_SWIZZLING3(y, w, x);        \
        VEC_SWIZZLING3(y, w, y);        \
        VEC_SWIZZLING3(y, w, z);        \
        VEC_SWIZZLING3(y, w, w);        \
        VEC_SWIZZLING3(z, x, x);        \
        VEC_SWIZZLING3(z, x, y);        \
        VEC_SWIZZLING3(z, x, z);        \
        VEC_SWIZZLING3(z, x, w);        \
        VEC_SWIZZLING3(z, y, x);        \
        VEC_SWIZZLING3(z, y, y);        \
        VEC_SWIZZLING3(z, y, z);        \
        VEC_SWIZZLING3(z, y, w);        \
        VEC_SWIZZLING3(z, z, x);        \
        VEC_SWIZZLING3(z, z, y);        \
        VEC_SWIZZLING3(z, z, z);        \
        VEC_SWIZZLING3(z, z, w);        \
        VEC_SWIZZLING3(z, w, x);        \
        VEC_SWIZZLING3(z, w, y);        \
        VEC_SWIZZLING3(z, w, z);        \
        VEC_SWIZZLING3(z, w, w);        \
        VEC_SWIZZLING3(w, x, x);        \
        VEC_SWIZZLING3(w, x, y);        \
        VEC_SWIZZLING3(w, x, z);        \
        VEC_SWIZZLING3(w, x, w);        \
        VEC_SWIZZLING3(w, y, x);        \
        VEC_SWIZZLING3(w, y, y);        \
        VEC_SWIZZLING3(w, y, z);        \
        VEC_SWIZZLING3(w, y, w);        \
        VEC_SWIZZLING3(w, z, x);        \
        VEC_SWIZZLING3(w, z, y);        \
        VEC_SWIZZLING3(w, z, z);        \
        VEC_SWIZZLING3(w, z, w);        \
        VEC_SWIZZLING3(w, w, x);        \
        VEC_SWIZZLING3(w, w, y);        \
        VEC_SWIZZLING3(w, w, z);        \
        VEC_SWIZZLING3(w, w, w);        \
        VEC_SWIZZLING4(x, x, x, x);     \
        VEC_SWIZZLING4(x, x, x, y);     \
        VEC_SWIZZLING4(x, x, x, z);     \
        VEC_SWIZZLING4(x, x, x, w);     \
        VEC_SWIZZLING4(x, x, y, x);     \
        VEC_SWIZZLING4(x, x, y, y);     \
        VEC_SWIZZLING4(x, x, y, z);     \
        VEC_SWIZZLING4(x, x, y, w);     \
        VEC_SWIZZLING4(x, x, z, x);     \
        VEC_SWIZZLING4(x, x, z, y);     \
        VEC_SWIZZLING4(x, x, z, z);     \
        VEC_SWIZZLING4(x, x, z, w);     \
        VEC_SWIZZLING4(x, x, w, x);     \
        VEC_SWIZZLING4(x, x, w, y);     \
        VEC_SWIZZLING4(x, x, w, z);     \
        VEC_SWIZZLING4(x, x, w, w);     \
        VEC_SWIZZLING4(x, y, x, x);     \
        VEC_SWIZZLING4(x, y, x, y);     \
        VEC_SWIZZLING4(x, y, x, z);     \
        VEC_SWIZZLING4(x, y, x, w);     \
        VEC_SWIZZLING4(x, y, y, x);     \
        VEC_SWIZZLING4(x, y, y, y);     \
        VEC_SWIZZLING4(x, y, y, z);     \
        VEC_SWIZZLING4(x, y, y, w);     \
        VEC_SWIZZLING4(x, y, z, x);     \
        VEC_SWIZZLING4(x, y, z, y);     \
        VEC_SWIZZLING4(x, y, z, z);     \
        VEC_SWIZZLING4(x, y, z, w);     \
        VEC_SWIZZLING4(x, y, w, x);     \
        VEC_SWIZZLING4(x, y, w, y);     \
        VEC_SWIZZLING4(x, y, w, z);     \
        VEC_SWIZZLING4(x, y, w, w);     \
        VEC_SWIZZLING4(x, z, x, x);     \
        VEC_SWIZZLING4(x, z, x, y);     \
        VEC_SWIZZLING4(x, z, x, z);     \
        VEC_SWIZZLING4(x, z, x, w);     \
        VEC_SWIZZLING4(x, z, y, x);     \
        VEC_SWIZZLING4(x, z, y, y);     \
        VEC_SWIZZLING4(x, z, y, z);     \
        VEC_SWIZZLING4(x, z, y, w);     \
        VEC_SWIZZLING4(x, z, z, x);     \
        VEC_SWIZZLING4(x, z, z, y);     \
        VEC_SWIZZLING4(x, z, z, z);     \
        VEC_SWIZZLING4(x, z, z, w);     \
        VEC_SWIZZLING4(x, z, w, x);     \
        VEC_SWIZZLING4(x, z, w, y);     \
        VEC_SWIZZLING4(x, z, w, z);     \
        VEC_SWIZZLING4(x, z, w, w);     \
        VEC_SWIZZLING4(x, w, x, x);     \
        VEC_SWIZZLING4(x, w, x, y);     \
        VEC_SWIZZLING4(x, w, x, z);     \
        VEC_SWIZZLING4(x, w, x, w);     \
        VEC_SWIZZLING4(x, w, y, x);     \
        VEC_SWIZZLING4(x, w, y, y);     \
        VEC_SWIZZLING4(x, w, y, z);     \
        VEC_SWIZZLING4(x, w, y, w);     \
        VEC_SWIZZLING4(x, w, z, x);     \
        VEC_SWIZZLING4(x, w, z, y);     \
        VEC_SWIZZLING4(x, w, z, z);     \
        VEC_SWIZZLING4(x, w, z, w);     \
        VEC_SWIZZLING4(x, w, w, x);     \
        VEC_SWIZZLING4(x, w, w, y);     \
        VEC_SWIZZLING4(x, w, w, z);     \
        VEC_SWIZZLING4(x, w, w, w);     \
        VEC_SWIZZLING4(y, x, x, x);     \
        VEC_SWIZZLING4(y, x, x, y);     \
        VEC_SWIZZLING4(y, x, x, z);     \
        VEC_SWIZZLING4(y, x, x, w);     \
        VEC_SWIZZLING4(y, x, y, x);     \
        VEC_SWIZZLING4(y, x, y, y);     \
        VEC_SWIZZLING4(y, x, y, z);     \
        VEC_SWIZZLING4(y, x, y, w);     \
        VEC_SWIZZLING4(y, x, z, x);     \
        VEC_SWIZZLING4(y, x, z, y);     \
        VEC_SWIZZLING4(y, x, z, z);     \
        VEC_SWIZZLING4(y, x, z, w);     \
        VEC_SWIZZLING4(y, x, w, x);     \
        VEC_SWIZZLING4(y, x, w, y);     \
        VEC_SWIZZLING4(y, x, w, z);     \
        VEC_SWIZZLING4(y, x, w, w);     \
        VEC_SWIZZLING4(y, y, x, x);     \
        VEC_SWIZZLING4(y, y, x, y);     \
        VEC_SWIZZLING4(y, y, x, z);     \
        VEC_SWIZZLING4(y, y, x, w);     \
        VEC_SWIZZLING4(y, y, y, x);     \
        VEC_SWIZZLING4(y, y, y, y);     \
        VEC_SWIZZLING4(y, y, y, z);     \
        VEC_SWIZZLING4(y, y, y, w);     \
        VEC_SWIZZLING4(y, y, z, x);     \
        VEC_SWIZZLING4(y, y, z, y);     \
        VEC_SWIZZLING4(y, y, z, z);     \
        VEC_SWIZZLING4(y, y, z, w);     \
        VEC_SWIZZLING4(y, y, w, x);     \
        VEC_SWIZZLING4(y, y, w, y);     \
        VEC_SWIZZLING4(y, y, w, z);     \
        VEC_SWIZZLING4(y, y, w, w);     \
        VEC_SWIZZLING4(y, z, x, x);     \
        VEC_SWIZZLING4(y, z, x, y);     \
        VEC_SWIZZLING4(y, z, x, z);     \
        VEC_SWIZZLING4(y, z, x, w);     \
        VEC_SWIZZLING4(y, z, y, x);     \
        VEC_SWIZZLING4(y, z, y, y);     \
        VEC_SWIZZLING4(y, z, y, z);     \
        VEC_SWIZZLING4(y, z, y, w);     \
        VEC_SWIZZLING4(y, z, z, x);     \
        VEC_SWIZZLING4(y, z, z, y);     \
        VEC_SWIZZLING4(y, z, z, z);     \
        VEC_SWIZZLING4(y, z, z, w);     \
        VEC_SWIZZLING4(y, z, w, x);     \
        VEC_SWIZZLING4(y, z, w, y);     \
        VEC_SWIZZLING4(y, z, w, z);     \
        VEC_SWIZZLING4(y, z, w, w);     \
        VEC_SWIZZLING4(y, w, x, x);     \
        VEC_SWIZZLING4(y, w, x, y);     \
        VEC_SWIZZLING4(y, w, x, z);     \
        VEC_SWIZZLING4(y, w, x, w);     \
        VEC_SWIZZLING4(y, w, y, x);     \
        VEC_SWIZZLING4(y, w, y, y);     \
        VEC_SWIZZLING4(y, w, y, z);     \
        VEC_SWIZZLING4(y, w, y, w);     \
        VEC_SWIZZLING4(y, w, z, x);     \
        VEC_SWIZZLING4(y, w, z, y);     \
        VEC_SWIZZLING4(y, w, z, z);     \
        VEC_SWIZZLING4(y, w, z, w);     \
        VEC_SWIZZLING4(y, w, w, x);     \
        VEC_SWIZZLING4(y, w, w, y);     \
        VEC_SWIZZLING4(y, w, w, z);     \
        VEC_SWIZZLING4(y, w, w, w);     \
        VEC_SWIZZLING4(z, x, x, x);     \
        VEC_SWIZZLING4(z, x, x, y);     \
        VEC_SWIZZLING4(z, x, x, z);     \
        VEC_SWIZZLING4(z, x, x, w);     \
        VEC_SWIZZLING4(z, x, y, x);     \
        VEC_SWIZZLING4(z, x, y, y);     \
        VEC_SWIZZLING4(z, x, y, z);     \
        VEC_SWIZZLING4(z, x, y, w);     \
        VEC_SWIZZLING4(z, x, z, x);     \
        VEC_SWIZZLING4(z, x, z, y);     \
        VEC_SWIZZLING4(z, x, z, z);     \
        VEC_SWIZZLING4(z, x, z, w);     \
        VEC_SWIZZLING4(z, x, w, x);     \
        VEC_SWIZZLING4(z, x, w, y);     \
        VEC_SWIZZLING4(z, x, w, z);     \
        VEC_SWIZZLING4(z, x, w, w);     \
        VEC_SWIZZLING4(z, y, x, x);     \
        VEC_SWIZZLING4(z, y, x, y);     \
        VEC_SWIZZLING4(z, y, x, z);     \
        VEC_SWIZZLING4(z, y, x, w);     \
        VEC_SWIZZLING4(z, y, y, x);     \
        VEC_SWIZZLING4(z, y, y, y);     \
        VEC_SWIZZLING4(z, y, y, z);     \
        VEC_SWIZZLING4(z, y, y, w);     \
        VEC_SWIZZLING4(z, y, z, x);     \
        VEC_SWIZZLING4(z, y, z, y);     \
        VEC_SWIZZLING4(z, y, z, z);     \
        VEC_SWIZZLING4(z, y, z, w);     \
        VEC_SWIZZLING4(z, y, w, x);     \
        VEC_SWIZZLING4(z, y, w, y);     \
        VEC_SWIZZLING4(z, y, w, z);     \
        VEC_SWIZZLING4(z, y, w, w);     \
        VEC_SWIZZLING4(z, z, x, x);     \
        VEC_SWIZZLING4(z, z, x, y);     \
        VEC_SWIZZLING4(z, z, x, z);     \
        VEC_SWIZZLING4(z, z, x, w);     \
        VEC_SWIZZLING4(z, z, y, x);     \
        VEC_SWIZZLING4(z, z, y, y);     \
        VEC_SWIZZLING4(z, z, y, z);     \
        VEC_SWIZZLING4(z, z, y, w);     \
        VEC_SWIZZLING4(z, z, z, x);     \
        VEC_SWIZZLING4(z, z, z, y);     \
        VEC_SWIZZLING4(z, z, z, z);     \
        VEC_SWIZZLING4(z, z, z, w);     \
        VEC_SWIZZLING4(z, z, w, x);     \
        VEC_SWIZZLING4(z, z, w, y);     \
        VEC_SWIZZLING4(z, z, w, z);     \
        VEC_SWIZZLING4(z, z, w, w);     \
        VEC_SWIZZLING4(z, w, x, x);     \
        VEC_SWIZZLING4(z, w, x, y);     \
        VEC_SWIZZLING4(z, w, x, z);     \
        VEC_SWIZZLING4(z, w, x, w);     \
        VEC_SWIZZLING4(z, w, y, x);     \
        VEC_SWIZZLING4(z, w, y, y);     \
        VEC_SWIZZLING4(z, w, y, z);     \
        VEC_SWIZZLING4(z, w, y, w);     \
        VEC_SWIZZLING4(z, w, z, x);     \
        VEC_SWIZZLING4(z, w, z, y);     \
        VEC_SWIZZLING4(z, w, z, z);     \
        VEC_SWIZZLING4(z, w, z, w);     \
        VEC_SWIZZLING4(z, w, w, x);     \
        VEC_SWIZZLING4(z, w, w, y);     \
        VEC_SWIZZLING4(z, w, w, z);     \
        VEC_SWIZZLING4(z, w, w, w);     \
        VEC_SWIZZLING4(w, x, x, x);     \
        VEC_SWIZZLING4(w, x, x, y);     \
        VEC_SWIZZLING4(w, x, x, z);     \
        VEC_SWIZZLING4(w, x, x, w);     \
        VEC_SWIZZLING4(w, x, y, x);     \
        VEC_SWIZZLING4(w, x, y, y);     \
        VEC_SWIZZLING4(w, x, y, z);     \
        VEC_SWIZZLING4(w, x, y, w);     \
        VEC_SWIZZLING4(w, x, z, x);     \
        VEC_SWIZZLING4(w, x, z, y);     \
        VEC_SWIZZLING4(w, x, z, z);     \
        VEC_SWIZZLING4(w, x, z, w);     \
        VEC_SWIZZLING4(w, x, w, x);     \
        VEC_SWIZZLING4(w, x, w, y);     \
        VEC_SWIZZLING4(w, x, w, z);     \
        VEC_SWIZZLING4(w, x, w, w);     \
        VEC_SWIZZLING4(w, y, x, x);     \
        VEC_SWIZZLING4(w, y, x, y);     \
        VEC_SWIZZLING4(w, y, x, z);     \
        VEC_SWIZZLING4(w, y, x, w);     \
        VEC_SWIZZLING4(w, y, y, x);     \
        VEC_SWIZZLING4(w, y, y, y);     \
        VEC_SWIZZLING4(w, y, y, z);     \
        VEC_SWIZZLING4(w, y, y, w);     \
        VEC_SWIZZLING4(w, y, z, x);     \
        VEC_SWIZZLING4(w, y, z, y);     \
        VEC_SWIZZLING4(w, y, z, z);     \
        VEC_SWIZZLING4(w, y, z, w);     \
        VEC_SWIZZLING4(w, y, w, x);     \
        VEC_SWIZZLING4(w, y, w, y);     \
        VEC_SWIZZLING4(w, y, w, z);     \
        VEC_SWIZZLING4(w, y, w, w);     \
        VEC_SWIZZLING4(w, z, x, x);     \
        VEC_SWIZZLING4(w, z, x, y);     \
        VEC_SWIZZLING4(w, z, x, z);     \
        VEC_SWIZZLING4(w, z, x, w);     \
        VEC_SWIZZLING4(w, z, y, x);     \
        VEC_SWIZZLING4(w, z, y, y);     \
        VEC_SWIZZLING4(w, z, y, z);     \
        VEC_SWIZZLING4(w, z, y, w);     \
        VEC_SWIZZLING4(w, z, z, x);     \
        VEC_SWIZZLING4(w, z, z, y);     \
        VEC_SWIZZLING4(w, z, z, z);     \
        VEC_SWIZZLING4(w, z, z, w);     \
        VEC_SWIZZLING4(w, z, w, x);     \
        VEC_SWIZZLING4(w, z, w, y);     \
        VEC_SWIZZLING4(w, z, w, z);     \
        VEC_SWIZZLING4(w, z, w, w);     \
        VEC_SWIZZLING4(w, w, x, x);     \
        VEC_SWIZZLING4(w, w, x, y);     \
        VEC_SWIZZLING4(w, w, x, z);     \
        VEC_SWIZZLING4(w, w, x, w);     \
        VEC_SWIZZLING4(w, w, y, x);     \
        VEC_SWIZZLING4(w, w, y, y);     \
        VEC_SWIZZLING4(w, w, y, z);     \
        VEC_SWIZZLING4(w, w, y, w);     \
        VEC_SWIZZLING4(w, w, z, x);     \
        VEC_SWIZZLING4(w, w, z, y);     \
        VEC_SWIZZLING4(w, w, z, z);     \
        VEC_SWIZZLING4(w, w, z, w);     \
        VEC_SWIZZLING4(w, w, w, x);     \
        VEC_SWIZZLING4(w, w, w, y);     \
        VEC_SWIZZLING4(w, w, w, z);     \
        VEC_SWIZZLING4(w, w, w, w);     \

#if GLSL_VEC_XYZW == 1
        DEF_VEC_FIELDS(x, y, z, w)
#endif
#if GLSL_VEC_RGBA == 1
        DEF_VEC_FIELDS(r, g, b, a)
#endif
#if GLSL_VEC_STPQ == 1
        DEF_VEC_FIELDS(s, t, p, q)
#endif
#undef DEF_VEC_FIELDS
    };
};

#endif // GLSL_VEC_SWIZZLING

#undef VEC_INDEX_y
#undef VEC_INDEX_z
#undef VEC_INDEX_x
#undef VEC_INDEX_w
#undef VEC_INDEX_r
#undef VEC_INDEX_g
#undef VEC_INDEX_b
#undef VEC_INDEX_a
#undef VEC_INDEX_s
#undef VEC_INDEX_t
#undef VEC_INDEX_p
#undef VEC_INDEX_q
#undef VEC_INDEX
#undef VEC_PROXY
#undef VEC_SWIZZLING1
#undef VEC_SWIZZLING2
#undef VEC_SWIZZLING3
#undef VEC_SWIZZLING4

#undef DEF_BASE

}
