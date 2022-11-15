#ifndef KURO_MATH_VEC_H__
#define KURO_MATH_VEC_H__

#include <array>
#include <cmath>
#include <iostream>
#include <stddef.h>
#include <type_traits>

#include "kuro/util/assert.hh"

namespace kuro {

#define VEC_COMMON_DEFINITION(d)                                               \
  Vec() noexcept = default;                                                    \
  ~Vec() noexcept = default;                                                   \
                                                                               \
  size_t GetDimension() const noexcept { return d; }                           \
                                                                               \
  Vec(std::array<T, d> const &data)                                            \
  {                                                                            \
    for (size_t i = 0; i < data_.size(); ++i) {                                \
      data_[i] = data[i];                                                      \
    }                                                                          \
  }                                                                            \
                                                                               \
  T &operator[](size_t i) noexcept { return data_[i]; }                        \
                                                                               \
  T const &operator[](size_t i) const noexcept { return data_[i]; }            \
                                                                               \
  float len() const noexcept                                                   \
  {                                                                            \
    T sum{};                                                                   \
    for (size_t i = 0; i < d; ++i) {                                           \
      sum += (data_[i] * data_[i]);                                            \
    }                                                                          \
                                                                               \
    return std::sqrt(sum);                                                     \
  }                                                                            \
                                                                               \
  Vec<float, d> Normalize() const noexcept                                     \
  {                                                                            \
    std::array<float, d> intermediate;                                         \
    for (size_t i = 0; i < d; ++i)                                             \
      intermediate[i] = data_[i];                                              \
    auto const l = len();                                                      \
    for (auto &e : intermediate) {                                             \
      e /= l;                                                                  \
    }                                                                          \
                                                                               \
    return Vec<float, d>(intermediate);                                        \
  }                                                                            \
                                                                               \
  void Print() const noexcept                                                  \
  {                                                                            \
    std::cout << "(";                                                          \
    for (size_t i = 0; i < d - 1; ++i)                                         \
      std::cout << data_[i] << ",";                                            \
    std::cout << data_[d - 1] << ")\n";                                        \
  }                                                                            \
                                                                               \
  Vec &operator+=(Vec const &rhs) noexcept                                     \
  {                                                                            \
    for (size_t i = 0; i < d; ++i) {                                           \
      data_[i] += rhs[i];                                                      \
    }                                                                          \
    return *this;                                                              \
  }                                                                            \
                                                                               \
  void MakeZero() noexcept                                                     \
  {                                                                            \
    for (size_t i = 0; i < d; ++i) {                                           \
      data_[i] = 0;                                                            \
    }                                                                          \
  }                                                                            \
                                                                               \
  void Fill(T val) noexcept                                                    \
  {                                                                            \
    for (int i = 0; i < d; ++i) {                                              \
      data_[i] = val;                                                          \
    }                                                                          \
  }

template <typename T, size_t N>
class Vec {
  static_assert(N > 0, "Demension must greater than 0");

 public:
  VEC_COMMON_DEFINITION(N)
 private:
  std::array<T, N> data_;
};

template <typename T>
class Vec<T, 2> {
 public:
  Vec(T x, T y)
  {
    data_[0] = x;
    data_[1] = y;
  }

  VEC_COMMON_DEFINITION(2)

  T x() const noexcept { return data_[0]; }
  T y() const noexcept { return data_[1]; }

 private:
  std::array<T, 2> data_;
};

template <typename T>
class Vec<T, 3> {
 public:
  Vec(T x, T y, T z)
  {
    data_[0] = x;
    data_[1] = y;
    data_[2] = z;
  }

  VEC_COMMON_DEFINITION(3)

  T x() const noexcept { return data_[0]; }
  T y() const noexcept { return data_[1]; }
  T z() const noexcept { return data_[2]; }

 private:
  std::array<T, 3> data_;
};

template <typename T>
class Vec<T, 4> {
 public:
  Vec(T x, T y, T z, T w)
  {
    data_[0] = x;
    data_[1] = y;
    data_[2] = z;
    data_[3] = w;
  }

  VEC_COMMON_DEFINITION(4)

  T x() const noexcept { return data_[0]; }
  T y() const noexcept { return data_[1]; }
  T z() const noexcept { return data_[2]; }
  T w() const noexcept { return data_[3]; }

 private:
  std::array<T, 4> data_;
};

/* Convenient alias */
using Vec2i = Vec<int, 2>;
using Vec3i = Vec<int, 3>;
using Vec4i = Vec<int, 4>;
using Vec2f = Vec<float, 2>;
using Vec3f = Vec<float, 3>;
using Vec4f = Vec<float, 4>;

template <typename T>
Vec<T, 3> CrossProduct3(Vec<T, 3> const &a, Vec<T, 3> const &b) noexcept
{
  /*
   * a x b =
   * | i   j    k  |
   * | a.x a.y a.z |
   * | b.x b.y b.z |
   */
  return Vec<T, 3>(a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(),
                   a.x() * b.y() - a.y() * b.x());
}

template <typename T, size_t N>
Vec<float, N> ToVecf(Vec<T, N> const &a) noexcept
{
  Vec<float, N> ret;
  for (size_t i = 0; i < N; ++i)
    ret[i] = (float)a[i];
  return ret;
}

template <typename T, size_t N>
Vec<int, N> ToVeci(Vec<T, N> const &v) noexcept
{
  Vec<int, N> ret;
  for (size_t i = 0; i < N; ++i) {
    ret[i] = (int)v[i];
  }
  return ret;
}
template <typename T, size_t N>
T DotProduct(Vec<T, N> const &a, Vec<T, N> const &b) noexcept
{
  T ret{};
  for (size_t i = 0; i < N; ++i) {
    ret += a[i] * b[i];
  }
  return ret;
}

template <typename T, size_t N>
bool IsOrthogonal(Vec<T, N> const &a, Vec<T, N> const &b) noexcept
{
  return DotProduct(a, b) == 0;
}

template <typename T, size_t N>
Vec<T, N> operator-(Vec<T, N> const &a, Vec<T, N> const &b) noexcept
{
  std::array<T, N> data;
  for (size_t i = 0; i < N; ++i) {
    data[i] = a[i] - b[i];
  }

  return Vec<T, N>(data);
}

template <typename T, size_t N>
Vec<T, N> operator-(Vec<T, N> const &a) noexcept
{
  Vec<T, N> ret;
  for (size_t i = 0; i < N; ++i)
    ret[i] = -a[i];
  return ret;
}

template <typename T, size_t N>
Vec<T, N> operator+(Vec<T, N> const &a, Vec<T, N> const &b) noexcept
{
  std::array<T, N> data;
  for (size_t i = 0; i < N; ++i) {
    data[i] = a[i] + b[i];
  }

  return Vec<T, N>(data);
}

template <
    typename T, size_t N, typename U,
    typename = typename std::enable_if<std::is_convertible<U, T>::value>::type>
inline Vec<T, N> operator/(Vec<T, N> const &a, U d) noexcept
{
  Vec<T, N> ret;
  for (size_t i = 0; i < N; ++i) {
    ret[i] = a[i] / d;
  }
  return ret;
}

template <
    typename T, size_t N, typename U,
    typename = typename std::enable_if<std::is_convertible<U, T>::value>::type>
inline Vec<T, N> operator/(U d, Vec<T, N> const &a) noexcept
{
  Vec<T, N> ret;
  for (size_t i = 0; i < N; ++i) {
    ret[i] = d / a[i];
  }
  return ret;
}

template <typename T, size_t N>
inline Vec<T, N> operator*(Vec<T, N> const &a, T m) noexcept
{
  Vec<T, N> ret;
  for (size_t i = 0; i < N; ++i) {
    ret[i] = a[i] * m;
  }
  return ret;
}

// TODO Use TMP optimization
template <size_t N, size_t F, typename T>
Vec<T, N> ClipVec(Vec<T, F> const &v) noexcept
{
  static_assert(N < F, "");
  Vec<T, N> ret;
  for (size_t i = 0; i < N; ++i) {
    ret[i] = v[i];
  }
  return ret;
}

template <size_t N, size_t F, typename T>
Vec<T, N> EmbedVec(Vec<T, F> const &v, T e) noexcept
{
  static_assert(N > F, "");
  Vec<T, N> ret;
  size_t i = 0;
  for (i = 0; i < F; ++i) {
    ret[i] = v[i];
  }
  for (; i < N; ++i) {
    ret[i] = e;
  }
  return ret;
}

template <size_t N, size_t F>
inline Vec<float, N> EmbedVecf(Vec<float, F> const &v, float e) noexcept
{
  return EmbedVec<N, F, float>(v, e);
}

} // namespace kuro

#endif
