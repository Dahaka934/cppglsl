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

private:

    template<class... Args>
    constexpr static bool constructibleFrom() {
        using trait = traits::vector_trait<Vector>;

        constexpr bool same_size = (traits::vector_size_for<Args, Vector>() + ...) == trait::size;
        constexpr bool same_space = ((traits::vector_trait<Args>::space <= trait::space) && ...);

        return same_size && same_space;
    }

public: // CONSTRUCTORS

    constexpr Vector() = default;

    constexpr Vector(const Vector&) = default;

    constexpr Vector(Vector&&) noexcept = default;

    template<concepts::SuitedScalarFor<Vector> T>
    constexpr Vector(T&& scalar) {
        foreachWith(scalar, [&](auto&& v1, auto&& v2) {
            v1 = v2;
        });
    }

    template<class... Args>
    constexpr Vector(Args&& ... args)
    requires (sizeof...(Args) > 0 && constructibleFrom<Args...>()) {
        construct<0>(std::forward<Args>(args)...);
    }

public: // OPERATORS

    constexpr ScalarRef operator[](size_t i) {
        return data[i];
    }

    constexpr ScalarArg operator[](size_t i) const {
        return data[i];
    }

public: // STL COMPATIBILITY

    ScalarRef at(size_t i) { return data.at(i); }

    ScalarArg at(size_t i) const { return data.at(i); }

    auto cbegin() const { return data.cbegin(); }

    auto cend() const { return data.cend(); }

    auto crbegin() const { return data.crbegin(); }

    auto crend() const { return data.crend(); }

    auto begin() const { return data.begin(); }

    auto end() const { return data.end(); }

    auto begin() { return data.begin(); }

    auto end() { return data.end(); }

    auto rbegin() const { return data.rbegin(); }

    auto rend() const { return data.rend(); }

    auto rbegin() { return data.rbegin(); }

    auto rend() { return data.rend(); }

    friend std::ostream& operator<<(std::ostream& os, const Vector& obj) {
        os << '(';
        foreachIndex([&](auto i) {
            os << obj[i] << (i == VectorSize - 1 ? "" : ",");
        });
        return os << ')';
    }

public: // UTILS

    template<concepts::SuitedScalarFor<Vector> T, class Func>
    constexpr void foreachWith(const T& obj, const Func& func) {
        foreachIndex([&, obj = scalarFrom(obj)](auto index) {
            func(at(index), obj);
        });
    }

    template<concepts::SuitedVectorFor<Vector> T, class Func>
    constexpr void foreachWith(const T& obj, const Func& func) {
        foreachIndex([&](auto index) {
            func(at(index), scalarFrom(obj.at(index)));
        });
    }

    template<concepts::SuitedTypeFor<Vector> T, class Func>
    constexpr void foreachWith(const T& obj, const Func& func) const {
        const_cast<Vector&>(*this).foreachWith(obj, func);
    }

    template<class Func>
    static constexpr void foreachIndex(const Func& func) {
        details::static_foreach<0, VectorSize>(func);
    }

    template<class T>
    static constexpr T&& take(T&& non_vec, size_t index) {
        return std::forward<T>(non_vec);
    }

    template<class T>
    requires (concepts::Vector<std::remove_cvref_t<T>> &&
              traits::vector_trait<Vector>::space == traits::vector_trait<T>::space)
    static constexpr auto take(T&& vec, size_t index) -> decltype(vec[index]) {
        return vec[index];
    }

private:

    template<size_t Off, class T, class... Ts>
    constexpr void construct(T&& t, Ts&& ... ts) {
        constexpr size_t Len = traits::vector_size_for<T, Vector>();
        compose<Off, Len>(std::forward<T>(t));
        if constexpr (Off + Len < VectorSize && sizeof...(ts))
            construct<Off + Len>(std::forward<Ts>(ts)...);
    }

    template<size_t Off, size_t Len, class T>
    constexpr void compose(const T& v) {
        details::static_foreach<Off, Off + Len>([&](auto i) {
            at(i) = scalarFrom(take(v, i - Off));
        });
    }

    template<std::convertible_to<ScalarType> T>
    static constexpr ScalarType scalarFrom(T v) {
        return static_cast<ScalarType>(v);
    }
};

}
