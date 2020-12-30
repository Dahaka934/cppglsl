#pragma once

#include "utils.h"

namespace glsl {

template<class Trait, size_t... Indices>
struct VectorProxy {

    static constexpr size_t VectorSize = sizeof...(Indices);
    using VectorItem = typename Trait::ScalarType;

    using Vector = typename Trait::template Factory<VectorItem, VectorSize>;
    using Data = typename Trait::DataType;

    static_assert(concepts::Vector<Vector>);

    constexpr operator Vector() const {
        return decay();
    }

    template<class T>
    constexpr VectorProxy& operator=(T&& v) {
        return set(std::forward<T>(v));
    }

    template<class T>
    constexpr VectorProxy& operator+=(T&& v) {
        return set(decay() + std::forward<T>(v));
    }

    template<class T>
    constexpr VectorProxy& operator-=(T&& v) {
        return set(decay() - std::forward<T>(v));
    }

    template<class T>
    constexpr VectorProxy& operator*=(T&& v) {
        return set(decay() * std::forward<T>(v));
    }

    template<class T>
    constexpr VectorProxy& operator/=(T&& v) {
        return set(decay() / std::forward<T>(v));
    }

    template<class T>
    constexpr auto operator+(T&& v) const {
        return decay() + std::forward<T>(v);
    }

    template<class T>
    constexpr auto operator-(T&& v) const {
        return decay() - std::forward<T>(v);
    }

    template<class T>
    constexpr auto operator*(T&& v) const {
        return decay() * std::forward<T>(v);
    }

    template<class T>
    constexpr auto operator/(T&& v) const {
        return decay() / std::forward<T>(v);
    }

    template<class T>
    constexpr bool operator==(T&& v) const {
        return decay() == std::forward<T>(v);
    }

    template<class T>
    constexpr bool operator!=(T&& v) const {
        return decay() != std::forward<T>(v);
    }

    friend std::ostream& operator<<(std::ostream& os, const VectorProxy& obj) {
        return os << obj.decay();
    }

    constexpr typename Trait::ScalarRef operator[](size_t i) {
        return data[indices[i]];
    }

    constexpr typename Trait::ScalarArg operator[](size_t i) const {
        return data[indices[i]];
    }

private:
    Data data;

    static constexpr size_t indices[sizeof...(Indices)] = { Indices... };

    constexpr Vector decay() const {
        return Vector{data[Indices]...};
    }

    constexpr VectorProxy& set(const Vector& vec) {
        details::static_foreach<0, VectorSize>([&](auto i) {
            operator[](i) = vec[i];
        });
        return *this;
    }
};

} // namespace glsl
