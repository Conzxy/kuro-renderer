#ifndef KURO_UTIL_ENDIAN_H__
#define KURO_UTIL_ENDIAN_H__

namespace kuro {

bool is_little_endian() noexcept;

template <typename T>
T to_little_endian(T src) noexcept;

template <typename T>
T to_big_endian(T src) noexcept;

} // namespace kuro

#endif // KURO_UTIL_ENDIAN_H__
