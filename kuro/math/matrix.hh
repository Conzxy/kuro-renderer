#ifndef KURO_MATH_MATRIX_H__
#define KURO_MATH_MATRIX_H__

#include <stddef.h>
#include <array>

#include "vec.hh"

namespace kuro {

template <typename T, size_t R, size_t C>
class Matrix {
  using Row = std::array<T, R>;
 public:
  Matrix() noexcept;
  explicit Matrix(T const &init) noexcept;
  
  ~Matrix() noexcept;
   
  Row &operator[](size_t r) noexcept { return data_[r]; }
  Row const &operator[](size_t r) const noexcept { return data_[r]; }
  
  Vec<T, R> const &ToVec() const noexcept
  {
    return data_[0];
  }

  Vec<T, R> &ToVec() noexcept
  {
    return data_[0];
  }

  size_t row() const noexcept { return R; }
  size_t col() const noexcept { return C; }

 private:
  std::array<std::array<T, R>, C> data_; 
};

template <typename T, size_t M, size_t K, size_t N>
Matrix<T, M, N> MatrixMultiple(Matrix<T, M, K> const &a, Matrix<T, K, N> const &b) noexcept
{
  size_t m = 0;
  size_t k = 0;
  size_t n = 0;
  
  Matrix<T, M, N> c;
  for (; m < M; ++m) {
    for (; n < N; ++n) {
      for (; k < K; ++k) {
        c[m][n] += a[m][k] * b[k][n];
      }
    }
  }
  return c;
}

} // namespace kuro

#endif
