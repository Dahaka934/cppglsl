#pragma once

#include <type_traits>
#include <concepts>

namespace glsl {

namespace traits {

template<class T, class True, class False>
using if_class_t = std::conditional_t<std::is_class_v<T>, True, False>;

template<class T>
using ref_if_class_t = if_class_t<T, std::add_lvalue_reference_t<T>, T>;

template<class T>
using cref_if_class_t = if_class_t<T, std::add_lvalue_reference_t<std::add_const_t<T>>, T>;

template<class T>
using uref_if_class_t = if_class_t<T, std::add_rvalue_reference_t<T>, T>;

template<class T>
constexpr bool dependent_false = std::false_type::value;

} // namespace traits

namespace concepts {

template<typename T>
concept Vector = requires(const T o) {
    typename T::VectorItem;
    typename T::template VectorFactory<typename T::VectorItem, 1>;
    { T::VectorSize } -> std::convertible_to<std::size_t>;
    { o[0] } -> std::common_reference_with<typename T::VectorItem>;
};

template<typename T>
concept VectorMutable = requires(T o) {
    requires Vector<T>;
    { o[0] = std::declval<typename T::VectorItem>() };
};

template<typename T>
concept VectorUnref = Vector<std::remove_cvref_t<T>>;

template<typename T>
concept Matrix = requires(const T o) {
    typename T::MatrixItem;
    { T::MatrixColumns } -> std::convertible_to<std::size_t>;
    { T::MatrixRows } -> std::convertible_to<std::size_t>;
    { o[0] } -> concepts::VectorUnref;
};

template<typename T>
concept MatrixQuad = requires {
    requires Matrix<T>;
    requires T::MatrixColumns == T::MatrixRows;
};

template<typename T, size_t N>
concept MatrixQuadN = requires {
    requires MatrixQuad<T>;
    requires T::MatrixColumns == N;
};

} // namespace concepts

namespace traits {

template<class T>
constexpr size_t vector_space() {
    using Type = std::remove_cvref_t<T>;
    if constexpr (concepts::Vector<Type>)
        return vector_space<typename Type::VectorItem>() + 1;
    else
        return 0;
}

template<class T>
struct vector_trait {
    using type = T;
    using item = T;

    static constexpr size_t size = 1;
    static constexpr size_t space = 1;

    template<class Scalar, size_t Size>
    using factory = T;
};

template<concepts::Vector T>
struct vector_trait<T> {
    using type = T;
    using item = typename T::VectorItem;

    static constexpr size_t size = T::VectorSize;
    static constexpr size_t space = vector_space<T>();

    template<class Scalar, size_t Size>
    using factory = typename T::template VectorFactory<Scalar, Size>;
};

template<class T> requires (std::is_reference_v<T>)
struct vector_trait<T> : vector_trait<std::remove_reference_t<T>> {};

template<class T, concepts::Vector V>
constexpr size_t vector_size_for() {
    return vector_trait<T>::space >= vector_trait<V>::space ? vector_trait<T>::size : 1;
}

} // namespace traits

} // namespace glsl
