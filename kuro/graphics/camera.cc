#include "camera.hh"

#include <cmath>

#include "kuro/math/util.hh"
#include "kuro/math/util.hh"
#include "kuro/util/log.hh"
#include "transform.hh"

using namespace kuro;

inline static Vec2f GetPosDelta(Vec2i from, Vec2i to, int h) noexcept
{
  /*
   * The reason for why divide by h is that
   * the x coordiante is scaled in height in fact.
   *
   * x' = 2n / (r - l) * width * x
   *    = (2n / (aspect_ratio * (t - b))) * aspect_ratio * height * x
   *    = 2n / (t-b) * x
   *
   * Other reason is that the speed in y-axis and x-axis should be same.
   */
  return ToVecf(to - from) / h;
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
    motion.orbit_offset = GetPosDelta(ctx.orbit_begin, ctx.orbit_end, ctx.height);
    ctx.orbit_begin = ctx.orbit_end;
  }
  if (ctx.is_paning) {
    motion.pan_offset = GetPosDelta(ctx.pan_begin, ctx.pan_end, ctx.height);
    ctx.pan_begin = ctx.pan_end;
  }

  motion.dolly = ctx.dolly_exp;
  ctx.dolly_exp = 0;
  
  auto from_target = position_ - target_;
  auto from_camera = target_ - position_;

  pan_offset_ = CalcPanOffset(from_camera, motion);
  orbit_offset_ = CalcOrbitOffset(from_target, motion);
  target_ = target_ + pan_offset_;
  position_ = target_ + orbit_offset_;
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

  /*
   * Mouse cursor move to right 
   * == object right rotation 
   * == camera left rotation
   * == theta decrease
   *
   * Mouse cursor move to top(move to inside) 
   * == rotate to inside(away from camera)
   * == camara move to bottom
   * == phi increase
   */
  theta -= factor * motion.orbit_offset.x();

  /*
   * NOTICE 
   * The origin of coordiante system of Qt
   * in the left top corner of monitor
   */
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

  /*
   * mouse cursor move to left 
   * == camera move to left 
   * == object move to right
   *
   * mouse cursor move to top
   * == camera move to top
   * == object move to bottom
   */
  auto nx = CrossProduct3(up_, z);

  /*
   * NOTICE
   * cursor move to top ==> delta(pos.y) < 0.
   * Hence, take the positive y-axis vector.
   */
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
  return ::GetViewMatrix(target_, position_, up_);
}
