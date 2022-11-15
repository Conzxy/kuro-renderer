#ifndef KURO_MATH_MATRIX_H__
#define KURO_MATH_MATRIX_H__

#include <array>
#include <initializer_list>
#include <stddef.h>

#include <iostream>
#include "vec.hh"

namespace kuro {

template <typename T, size_t R, size_t C>
class Matrix {
  static_assert(std::is_arithmetic<T>::value,
                "The element type of matrix must be floating or integer!");

  using Row = std::array<T, C>;
  using Data = std::array<Row, R>;

 public:
  Matrix() noexcept = default;
  Matrix(Data const &data)
    : data_(data)
  {
  }

  Matrix(std::initializer_list<std::initializer_list<T>> data)
  {
    SetData(data);
  }
  
  template <typename = typename std::enable_if<
    !std::is_same<typename std::decay<T>::type, Matrix>::value
    >::type>
  explicit Matrix(T const &init) noexcept { 
    for (auto &row : data_)
      row.fill(init); 
  }
  
  Matrix(Matrix const &) = default;
  Matrix(Matrix &&) = default;
  Matrix &operator=(Matrix const &) = default;
  Matrix &operator=(Matrix &&) = default;

  ~Matrix() noexcept = default;

  Row &operator[](size_t r) noexcept { return data_[r]; }
  Row const &operator[](size_t r) const noexcept { return data_[r]; }

  Vec<T, R> const &ToVec() const noexcept { return data_[0]; }

  Vec<T, R> &ToVec() noexcept { return data_[0]; }

  size_t row() const noexcept { return R; }
  size_t col() const noexcept { return C; }
  
  void Print() const noexcept
  {
    for (auto &row : data_) {
      for (auto e : row)
        std::cout << e << ", ";
      std::cout << '\n';
    }
  }

  void SetData(std::initializer_list<std::initializer_list<T>> data)
  {
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j)
        data_[i][j] = *((data.begin()+i)->begin()+j);
    }
  }
 private:
  Data data_;
};

template <typename T, size_t M, size_t K, size_t N>
Matrix<T, M, N> MatrixMultiple(Matrix<T, M, K> const &a,
                               Matrix<T, K, N> const &b) noexcept
{
  Matrix<T, M, N> c(0);
  for (size_t m = 0; m < M; ++m) {
    for (size_t k = 0; k < K; ++k) {
      for (size_t n = 0; n < N; ++n) {
        c[m][n] += a[m][k] * b[k][n];
      }
    }
  }
  return c;
}

template <typename T, size_t M, size_t K, size_t N>
inline Matrix<T, M, N> operator*(Matrix<T, M, K> const &a,
                                 Matrix<T, K, N> const &b) noexcept
{
  return MatrixMultiple(a, b);
}

template <typename T, size_t M, size_t N>
Vec<T, M> MatrixMultiple(Matrix<T, M, N> const &a, Vec<T, N> const &b) noexcept
{
  Vec<T, M> ret;
  for (size_t i = 0; i < M; ++i) {
    ret[i] = 0;
    for (size_t j = 0; j < N; ++j) {
      ret[i] += a[i][j] * b[j];
    }
  }
  return ret;
}

template <typename T, size_t M, size_t N>
inline Vec<T, M> operator*(Matrix<T, M, N> const &a, Vec<T, N> const &b) noexcept
{
  return MatrixMultiple(a, b);
}

template <size_t N, typename T>
inline Matrix<T, N, N> GetIdentity() noexcept
{
  Matrix<T, N, N> ret;
  for (size_t i = 0; i < N; ++i) {
    ret[i][i] = 1;
  }
  return ret;
}

template <size_t N>
inline Matrix<float, N, N> GetIdentityF() noexcept
{
  return GetIdentity<N, float>();
}

template <typename T>
using Matrix4x4 = Matrix<T, 4, 4>;

using Matrix4x4i = Matrix4x4<int>;
using Matrix4x4f = Matrix4x4<float>;

} // namespace kuro

#endif
