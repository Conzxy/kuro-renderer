#ifndef KURO_GRAPHICS_CAMERA_H__
#define KURO_GRAPHICS_CAMERA_H__

#include "kuro/math/vec.hh"
#include "kuro/math/matrix.hh"

namespace kuro {

struct CameraContext {
  /*
   * window's width and height is variable,
   * we must store it every time
   */
  int width = 0;
  int height = 0;
  
  /*
   * Orbit context
   */
  bool is_orbiting = false;
  Vec2i orbit_begin = { 0, 0 };
  Vec2i orbit_end = { 0, 0 };
  
  /*
   * dolly context
   *
   * dolly_exp == 0 --> no dolly
   */
  float dolly_exp = 0;
  
  /*
   * pan context
   */
  bool is_paning = false;
  Vec2i pan_begin = { 0, 0 };
  Vec2i pan_end = { 0, 0 };
};

/*
 * motion information from camera context
 */
struct Motion {
  Vec2f orbit_offset = { 0, 0 };
  Vec2f pan_offset = { 0, 0 };
  float dolly = 0; 
};

/**
 * \brief Orbit camera supporting rotation, dolly, paning.
 *
 * Rotate around an object along a sphere(radius = distance(target, camera).
 * Dolly doesn't like zoom, it put the object closer or further.
 * Paning move the object in xy plane.
 *
 * \see https://github.com/zauonlok/renderer/blob/master/renderer/core/camera.c
 * \see https://observablehq.com/@mbostock/hello-three-js-orbit-controls
 */
class Camera {
 public:
  Camera(Vec3f target, Vec3f position, float aspect_ratio);
  
  void Update(CameraContext &context);
  
  void SetPosition(Vec3f position) noexcept
  {
    position_ = position;
  }

  void SetTarget(Vec3f target) noexcept
  {
    target_ = target;
  }
  
  void SetAspectRatio(float ar) noexcept
  {
    aspect_ratio_ = ar;
  }

  void SetFovY(float fovY) noexcept
  {
    fovY_ = fovY;
  }

  void ResetMotion() noexcept
  {
    pan_offset_.MakeZero();
    orbit_offset_.MakeZero();
  }

  Matrix4x4f GetProjectionMatrix();
  Matrix4x4f GetViewMatrix();
  
  Vec3f position() const noexcept { return position_; }
  Vec3f target() const noexcept { return target_; }

  float GetRadius() const noexcept { return (position_ - target_).len(); }
 private:
  Vec3f CalcOrbitOffset(Vec3f from_target, Motion const &motion);
  Vec3f CalcPanOffset(Vec3f from_camera, Motion const &motion);

  Vec3f target_;
  Vec3f position_;

  float aspect_ratio_;
  float near_;
  float far_;
  float fovY_;
  Vec3f up_;

  Vec3f orbit_offset_;
  Vec3f pan_offset_;
};

} // namespace kuro

#endif
