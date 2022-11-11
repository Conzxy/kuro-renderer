#ifndef KURO_GRAPHICS_TRANSFORM_H__
#define KURO_GRAPHICS_TRANSFORM_H__

#include "kuro/math/matrix.hh"

namespace kuro {

inline Matrix4x4f GetViewportMatrix(float w, float h) noexcept
{
  return {
    { w/2, 0, 0, w/2 },
    { 0, h/2, 0, h/2 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 1 }
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
  Matrix4x4f ret;
  float y = -near * std::tan(fovY / 2);
  float x = aspect_ratio * y;
  ret[0] = { near / x, 0, 0, 0 };
  ret[1] = { 0, near / y, 0, 0 };
  ret[2] = { 0, 0, (near+far)/(near-far), -2*near*far/(near-far) };
  ret[3] = { 0, 0, 1, 0 };

  return ret;

}

inline Matrix4x4f GetViewMatrix(Vec3f target, Vec3f position, Vec3f up) noexcept
{
  Vec3f z = (target - position).Normalize();
  Vec3f x = CrossProduct3(z, up).Normalize();
  Vec3f y = CrossProduct3(x, z).Normalize();
  
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
}

} // namespace kuro

#endif
