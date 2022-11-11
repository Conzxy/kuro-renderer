#include "camera.hh"

#include <cmath>

#include "kuro/math/util.hh"
#include "kuro/math/util.hh"
#include "kuro/util/log.hh"
#include "transform.hh"

using namespace kuro;

static Vec2f GetPosDelta(Vec2i from, Vec2i to, int w, int h)
{
  auto ret = ToVecf(to - from);
  ret[0] /= w;
  ret[1] /= h;
  return ret;
}

Camera::Camera(Vec3f target, Vec3f position, float aspect_ratio)
  : target_(target)
  , position_(position)
  , aspect_ratio_(aspect_ratio)
  , near_(-0.1)
  , far_(-10000)
  , fovY_(AngleToRadian(60))
  , up_(0, 1, 0)
{
}

void Camera::Update(CameraContext &ctx)
{
  Motion motion;
  if (ctx.is_orbiting) {
    motion.orbit_offset = GetPosDelta(ctx.pressd_pos, ctx.orbit_end, ctx.width, ctx.height);
    ctx.pressd_pos = ctx.orbit_end;
  }
  if (ctx.is_paning) {
    motion.pan_offset = GetPosDelta(ctx.pressd_pos, ctx.pan_end, ctx.width, ctx.height);
    ctx.pressd_pos = ctx.pan_end;
  }

  motion.dolly = ctx.dolly_exp;
  ctx.dolly_exp = 0;
  
  auto from_target = position_ - target_;
  auto from_camera = target_ - position_;

  offset_from_position_ = CalcPanOffset(from_camera, motion);
  offset_from_target_ = CalcOrbitOffset(from_target, motion);
  target_ = target_ + offset_from_position_;
  position_ = target_ + offset_from_target_;
}

Vec3f Camera::CalcOrbitOffset(Vec3f from_target, Motion const &motion)
{
  /*
   * 沿以target为球心的球面环绕物体
   */

  float r = from_target.len();
  
  /*
   * theta is the radian between the projection of radius and z
   * phi is the radian between y and r
   *
   * \see https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
   *
   * Don't use atan() to get the theta radian:
   * atan() return value in [-pi/2, pi/2]
   * atan2() return value in [-pi, pi]
   *
   * \see https://en.cppreference.com/w/cpp/numeric/math/atan2
   */
  float theta = (float)std::atan2(from_target.x(), from_target.z());
  float phi = (float)std::acos(from_target.y() / r);

  auto factor = 2 * KURO_PI;
  theta -= factor * motion.orbit_offset.x();
  phi -= factor * motion.orbit_offset.y();
  phi = Clamp<float>(phi, KURO_EPISION, KURO_PI-KURO_EPISION);
  r *= (float)std::pow(0.95, motion.dolly);

  Vec3f offset;
  offset[0] = r * (float)sin(phi) * (float)sin(theta);
  offset[1] = r * (float)cos(phi);
  offset[2] = r * (float)sin(phi) * (float)cos(theta);

  return offset;
}

Vec3f Camera::CalcPanOffset(Vec3f from_camera, Motion const &motion)
{
  auto len = from_camera.len();
  auto z = from_camera / len;
  auto nx = CrossProduct3(up_, z);
  auto y = CrossProduct3(z, nx);
  
  auto y_factor = 2 * (float)tan(fovY_ / 2) * len;
  auto x_factor = y_factor * aspect_ratio_;

  auto delta_x = nx * (motion.pan_offset.x() * x_factor);
  auto delta_y = y * (motion.pan_offset.y() * y_factor);

  return delta_x + delta_y;
}

Matrix4x4f Camera::GetProjectionMatrix()
{
  return ::GetProjectionMatrix(near_, far_, fovY_, aspect_ratio_);
}

Matrix4x4f Camera::GetViewMatrix()
{
  DebugPrintf("position: (%f, %f, %f)\n", position_[0], position_[1], position_[2]);
  return ::GetViewMatrix(target_, position_, up_);
}
