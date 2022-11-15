#ifndef KURO_GRAPHICS_TRANSFORM_H__
#define KURO_GRAPHICS_TRANSFORM_H__

#include <assert.h>

#include "kuro/math/matrix.hh"

namespace kuro {

/** 
 * **** Discard ****
 *
 * Reason: 
 * Compared to multiple matrix to get the coordinate, multiple directly is better.
 */
inline Matrix4x4f GetViewportMatrix(float w, float h) noexcept
{
  return {
    { w/2, 0, 0, w/2 },
    { 0, h/2, 0, h/2 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 1 }
  };
}

inline Vec3f GetViewPortCoordinate(Vec3f coor, float w, float h) noexcept
{
  return {
    (coor.x() + 1) * w / 2,
    (coor.y() + 1) * h / 2,
    coor.z()
  };
}

/**
 * \param near The distance between near plane and camera
 * \param far The distance between far plane and camera
 * \param fovY The field of view(Must be radian)
 * \param aspect_ratio The ratio of width and height
 */
inline Matrix4x4f GetProjectionMatrix(float near, float far, float fovY, float aspect_ratio) noexcept
{
  assert(near < 0 && "Near plane must be in the -Z axis");
  const auto tan_fovY = float(std::tan(fovY/2));
  return {
    { -1 / (aspect_ratio * tan_fovY), 0, 0, 0 },
    { 0, -1 / tan_fovY, 0, 0 },
    { 0, 0, (near+far)/(near-far), -2*near*far/(near-far) },
    { 0, 0, 1, 0 }
  };
}

inline Matrix4x4f GetViewMatrix(Vec3f target, Vec3f position, Vec3f up) noexcept
{
  Vec3f z = (target - position).Normalize();
  Vec3f x = CrossProduct3(z, up).Normalize();
  Vec3f y = CrossProduct3(x, z).Normalize();

#if 0
  Matrix4x4f rotation ({
    { x.x(), x.y(), x.z(), 0 },
    { y.x(), y.y(), y.z(), 0 },
    { -z.x(), -z.y(), -z.z(), 0 },
    { 0, 0, 0, 1 }
  });

  Matrix4x4f translation({
    { 1, 0, 0, -position.x() },
    { 0, 1, 0, -position.y() },
    { 0, 0, 1, -position.z() },
    { 0, 0, 0, 1 }
  });

  return rotation * translation;
#else
  Vec3f row1{ x.x(), x.y(), x.z() };
  Vec3f row2{ y.x(), y.y(), y.z() };
  Vec3f row3{ -z.x(), -z.y(), -z.z() };
  Vec3f col4{ -position.x(), -position.y(), -position.z() };

  return {
    { x.x(), x.y(), x.z(), DotProduct(row1, col4) },
    { y.x(), y.y(), y.z(), DotProduct(row2, col4) },
    { -z.x(), -z.y(), -z.z(), DotProduct(row3, col4) },
    { 0, 0, 0, 1 }
  };
#endif
}

} // namespace kuro

#endif
