#ifndef KURO_MATH_UTIL_H__
#define KURO_MATH_UTIL_H__

#include <cmath>
#include <type_traits>

namespace kuro {

#define KURO_EPISION 1e-2
#define KURO_PI M_PI

inline float AngleToRadian(float angle) noexcept
{
  return (angle * KURO_PI) / 180;
}

template <typename T, 
  typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
inline T Clamp(T value, T bottom, T top) noexcept
{
  return value < bottom ?
         bottom : 
         (value > top ? top : value);
}

template <typename T,
  typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
inline T Clamp(T value, T top) noexcept
{
  return value > top ? value : top;
}

} // namespace kuro

#endif
