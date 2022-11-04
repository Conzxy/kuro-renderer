#include "endian.hh"

#include <stdint.h>
#include <assert.h>

#include <algorithm>

namespace kuro {

bool is_little_endian() noexcept
{
  unsigned int integer = 1;
  return *(&integer) == 1;
}

template <typename T>
T to_little_endian(T src) noexcept
{
  if (!is_little_endian()) {
    std::reverse((char*)&src, (char*)(&src+1));
  }
  return src;
}

template <typename T>
T to_big_endian(T src) noexcept
{
  if (is_little_endian()) {
    std::reverse((char*)&src, (char*)(&src+1));
  }
  return src;
}

#define DEF_EXPLICIT_INSTANTIATION_OF_TO_LITTLE_ENDIAN(type) \
template type to_little_endian<type>(type src) noexcept

#define DEF_EXPLICIT_INSTANTIATION_OF_TO_BIG_ENDIAN(type) \
template type to_big_endian<type>(type src) noexcept

DEF_EXPLICIT_INSTANTIATION_OF_TO_LITTLE_ENDIAN(uint8_t);
DEF_EXPLICIT_INSTANTIATION_OF_TO_LITTLE_ENDIAN(uint16_t);
DEF_EXPLICIT_INSTANTIATION_OF_TO_LITTLE_ENDIAN(uint32_t);
DEF_EXPLICIT_INSTANTIATION_OF_TO_LITTLE_ENDIAN(uint64_t);
DEF_EXPLICIT_INSTANTIATION_OF_TO_LITTLE_ENDIAN(int8_t);
DEF_EXPLICIT_INSTANTIATION_OF_TO_LITTLE_ENDIAN(int16_t);
DEF_EXPLICIT_INSTANTIATION_OF_TO_LITTLE_ENDIAN(int32_t);
DEF_EXPLICIT_INSTANTIATION_OF_TO_LITTLE_ENDIAN(int64_t);

DEF_EXPLICIT_INSTANTIATION_OF_TO_BIG_ENDIAN(uint8_t);
DEF_EXPLICIT_INSTANTIATION_OF_TO_BIG_ENDIAN(uint16_t);
DEF_EXPLICIT_INSTANTIATION_OF_TO_BIG_ENDIAN(uint32_t);
DEF_EXPLICIT_INSTANTIATION_OF_TO_BIG_ENDIAN(uint64_t);
DEF_EXPLICIT_INSTANTIATION_OF_TO_BIG_ENDIAN(int8_t);
DEF_EXPLICIT_INSTANTIATION_OF_TO_BIG_ENDIAN(int16_t);
DEF_EXPLICIT_INSTANTIATION_OF_TO_BIG_ENDIAN(int32_t);
DEF_EXPLICIT_INSTANTIATION_OF_TO_BIG_ENDIAN(int64_t);

} // namespace kuro
