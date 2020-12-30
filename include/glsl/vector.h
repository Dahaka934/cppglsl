#pragma once

#include "details/vector_base.h"

namespace glsl {

template<class Scalar, size_t Size>
struct VectorTrait;

template<class Scalar, size_t Size, template<class, size_t> class Trait = VectorTrait>
struct Vector;

template<class Scalar, size_t Size>
struct VectorTrait {
    using ScalarType = Scalar;
    using ScalarRef = std::add_lvalue_reference_t<ScalarType>;
    using ScalarArg = traits::cref_if_class_t<ScalarType>;

    template<class T = Scalar, size_t S = Size>
    using Factory = Vector<T, S, VectorTrait>;

    using DataType = std::array<ScalarType, Size>;

    template<size_t... Indices>
    struct ProxyImpl {
        using type = VectorProxy<VectorTrait, Indices...>;
    };

    template<size_t Index>
    struct ProxyImpl<Index> {
        using type = ScalarType;
    };

    template<size_t... Indices>
    using Proxy = typename ProxyImpl<Indices...>::type;
};

template<class Scalar, size_t Size, template<class, size_t> class Trait>
struct Vector : VectorBase<Scalar, Size, Trait> {

    using Base = VectorBase<Scalar, Size, Trait>;
    using TraitType = Trait<Scalar, Size>;
    using ScalarType = typename TraitType::ScalarType;
    using ScalarArg = typename TraitType::ScalarArg;
    using ScalarRef = typename TraitType::ScalarRef;

    using VectorItem = ScalarType;

    template<class Scalar_, size_t Size_>
    using VectorFactory = Vector<Scalar_, Size_, Trait>;

    using Base::data;

    static constexpr size_t size = Size;
    static constexpr size_t VectorSize = Size;
};

}
