#pragma once

#include "vector.h"

namespace glsl {

template<class Scalar, size_t N, size_t M, template<class, size_t> class Trait = VectorTrait>
struct Matrix {

    using RowType = typename Trait<Scalar, M>::template Factory<>;
    using ColumnType = typename Trait<Scalar, N>::template Factory<>;
    using MatrixItem = Scalar;

    using ScalarType = typename ColumnType::ScalarType;
    using ScalarArg = typename ColumnType::ScalarArg;
    using ScalarRef = typename ColumnType::ScalarRef;

    static constexpr size_t MatrixColumns = M;
    static constexpr size_t MatrixRows = N;
    static constexpr size_t MatrixSize = M * N;

private:

    template<class... Args>
    constexpr static bool constructibleFrom() {
        using trait = traits::vector_trait<ColumnType>;

        constexpr bool same_size = (traits::vector_size_for<Args, ColumnType>() + ...) == MatrixSize;
        constexpr bool same_space = ((traits::vector_trait<Args>::space <= trait::space) && ...);

        return same_size && same_space;
    }

public: // CONSTRUCTORS

    constexpr Matrix() = default;

    constexpr Matrix(const Matrix&) = default;

    constexpr Matrix(Matrix&&) noexcept = default;

    template<std::convertible_to<Scalar> T>
    constexpr explicit Matrix(T&& scalar) {
        details::static_foreach<0, M>([&](size_t i) {
            column(i) = ColumnType(0);
        });
        details::static_foreach<0, std::min(N, M)>([&](size_t i) {
            at(i, i) = scalarFrom(scalar);
        });
    }

    template<size_t OtherM, size_t OtherN>
    constexpr explicit Matrix(const Matrix<Scalar, OtherN, OtherM>& other) : Matrix(1) {
        details::static_foreach<0, std::min(N, OtherN)>([&](size_t row) {
            details::static_foreach<0, std::min(M, OtherM)>([&](size_t col) {
                at(row, col) = other.at(row, col);
            });
        });
    }

    template<class... Args>
    constexpr explicit Matrix(Args&&... args)
    requires (sizeof...(Args) > 0 && constructibleFrom<Args...>()) {
        construct<0>(std::forward<Args>(args)...);
    }

public: // OPERATORS

    constexpr const ColumnType& operator[](size_t i) const {
        return data[i];
    }

    constexpr ColumnType& operator[](size_t i) {
        return data[i];
    }

    constexpr Matrix& operator=(const Matrix&) = default;

    constexpr Matrix& operator=(Matrix&&) noexcept = default;

    template<class T>
    requires (std::same_as<T, Matrix> || std::convertible_to<T, Scalar>)
    constexpr Matrix& operator+=(const T& v) {
        foreachColumn([&](auto i) {
            column(i) += take(v, i);
        });
        return *this;
    }

    template<class T>
    requires (std::same_as<T, Matrix> || std::convertible_to<T, Scalar>)
    constexpr Matrix& operator-=(const T& v) {
        foreachColumn([&](auto i) {
            column(i) -= take(v, i);
        });
        return *this;
    }

    template<class T>
    requires (std::same_as<T, Matrix> || std::convertible_to<T, Scalar>)
    constexpr Matrix& operator*=(const T& v) {
        foreachColumn([&](auto i) {
            column(i) *= take(v, i);
        });
        return *this;
    }

    template<class T>
    requires (std::same_as<T, Matrix> || std::convertible_to<T, Scalar>)
    constexpr Matrix& operator/=(const T& v) {
        foreachColumn([&](auto i) {
            column(i) /= take(v, i);
        });
        return *this;
    }

    constexpr Matrix operator+() {
        return *this;
    }

    constexpr Matrix operator-() {
        return Matrix(0) -= *this;
    }

    constexpr Matrix& operator++() {
        return operator+=(1);
    }

    constexpr Matrix& operator--() {
        return operator-=(1);
    }

    constexpr Matrix operator++(int) & {
        Matrix result(*this);
        operator++();
        return result;
    }

    constexpr Matrix operator--(int) & {
        Matrix result(*this);
        operator--();
        return result;
    }

    constexpr bool operator==(const Matrix& v) const {
        bool equals = true;
        foreachColumn([&](auto i) {
            equals &= column(i) == v[i];
        });
        return equals;
    }

    constexpr bool operator!=(const Matrix& v) const {
        return !operator==(v);
    }

    template<class T>
    requires (std::same_as<T, Matrix> || std::convertible_to<T, Scalar>)
    friend constexpr Matrix operator+(const Matrix& v1, const T& v2) {
        return Matrix(v1) += v2;
    }

    template<class T>
    requires (std::same_as<T, Matrix> || std::convertible_to<T, Scalar>)
    friend constexpr Matrix operator-(const Matrix& v1, const T& v2) {
        return Matrix(v1) -= v2;
    }

    template<class T>
    requires (std::same_as<T, Matrix> || std::convertible_to<T, Scalar>)
    friend constexpr Matrix operator*(const Matrix& v1, const T& v2) {
        return Matrix(v1) *= v2;
    }

    template<class T>
    requires (std::same_as<T, Matrix> || std::convertible_to<T, Scalar>)
    friend constexpr Matrix operator/(const Matrix& v1, const T& v2) {
        return Matrix(v1) /= v2;
    }

    template<class T>
    requires (std::convertible_to<T, Scalar>)
    friend constexpr Matrix operator+(const T& v1, const Matrix& v2) {
        return v2 + v1;
    }

    template<class T>
    requires (std::convertible_to<T, Scalar>)
    friend constexpr Matrix operator-(const T& v1, const Matrix& v2) {
        return v2 - v1;
    }

    template<class T>
    requires (std::convertible_to<T, Scalar>)
    friend constexpr Matrix operator*(const T& v1, const Matrix& v2) {
        return v2 * v1;
    }

    template<class T>
    requires (std::convertible_to<T, Scalar>)
    friend constexpr Matrix operator/(const T& v1, const Matrix& v2) {
        return v1 * inverse(v2);
    }

    friend constexpr ColumnType operator*(const Matrix& m1, const RowType& m2) {
        ColumnType result;
        details::static_foreach<0, N>([&](size_t row) {
            result[row] = dot(m1.row(row), m2);
        });
        return result;
    }

    friend constexpr RowType operator*(const ColumnType& m1, const Matrix& m2) {
        RowType result;
        details::static_foreach<0, M>([&](size_t col) {
            result[col] = dot(m1, m2.column(col));
        });
        return result;
    }

    template<size_t OtherM> requires (OtherM != M)
    friend constexpr auto operator*(const Matrix& m1, const Matrix<Scalar, M, OtherM, Trait>& m2) {
        Matrix<Scalar, N, OtherM, Trait> result;
        details::static_foreach<0, OtherM>([&](size_t col) {
            result.column(col) = m1 * m2.column(col);
        });
        return result;
    }

    constexpr operator ScalarType() const
    requires (M == 1 && N == 1) {
        return at(0, 0);
    }

public: // AUXILIARY

    constexpr const ColumnType& column(size_t i) const {
        return data[i];
    }

    constexpr ColumnType& column(size_t i) {
        return data[i];
    }

    constexpr RowType row(size_t i) const {
        RowType result{};
        foreachColumn([&](auto col) {
            result[col] = at(i, col);
        });
        return result;
    }

    constexpr ScalarArg at(size_t row, size_t col) const {
        return data[col][row];
    }

    constexpr ScalarRef at(size_t row, size_t col) {
        return data[col][row];
    }

    constexpr ScalarArg at(size_t index) const {
        return at(index % N, index / N);
    }

    constexpr ScalarRef at(size_t index) {
        return at(index % N, index / N);
    }

public: // STL COMPATIBILITY

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

    friend std::ostream& operator<<(std::ostream& os, const Matrix& obj) {
        os << '[';
        foreachColumn([&](auto i) {
            os << obj[i] << (i == M - 1 ? "" : ",");
        });
        return os << ']';
    }

public: // UTILS

    template<class Func>
    static constexpr void foreachColumn(const Func& func) {
        details::static_foreach<0, M>(func);
    }

    template<class T> requires (std::convertible_to<std::remove_cvref_t<T>, Scalar>)
    static constexpr T&& take(T&& obj, size_t index) {
        return std::forward<T>(obj);
    }

    template<class T> requires (std::same_as<std::remove_cvref_t<T>, Matrix>)
    static constexpr auto take(T&& obj, size_t index) -> decltype(obj[index]) {
        return obj[index];
    }

private:
    typename VectorTrait<ColumnType, M>::DataType data;

    template<size_t Off, class T, class... Ts>
    constexpr void construct(T&& t, Ts&& ... ts) {
        constexpr size_t Len = traits::vector_size_for<T, ColumnType>();
        compose<Off, Len>(std::forward<T>(t));
        if constexpr (Off + Len < MatrixSize && sizeof...(ts))
            construct<Off + Len>(std::forward<Ts>(ts)...);
    }

    template<size_t Off, size_t Len, class T>
    constexpr void compose(const T& v) {
        details::static_foreach<Off, Off + Len>([&](auto i) {
            at(i) = scalarFrom(ColumnType::take(v, i - Off));
        });
    }

    template<std::convertible_to<ScalarType> T>
    static constexpr ScalarType scalarFrom(T v) {
        return static_cast<ScalarType>(v);
    }
};

}
