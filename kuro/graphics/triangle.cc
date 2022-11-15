#include "triangle.hh"

#include "kuro/graphics/shader_interface.hh"
#include "kuro/util/log.hh"

#include <random>
#include <ctime>
#include <climits>
#include <tuple>

namespace kuro {

static std::tuple<Vec2f, Vec2f> GetBoundingBox(Vec2f const* vtxs, Vec2f clamp)
{
  Vec2f bbmin(vtxs[0].x(), vtxs[0].y());
  Vec2f bbmax(vtxs[0].x(), vtxs[0].y());

  for (int i = 1; i < 3; ++i) {
    auto const &vertex = vtxs[i];
    for (int j = 0; j < 2; ++j) {
      bbmin[j] = std::min(bbmin[j], vertex[j]);
      bbmax[j] = std::min(clamp[j], std::max(bbmax[j], vertex[j]));
    }
  }
#if 0
  printf("Bounding box: \n");
  printf("(%d, %d)\n", bbmin[0], bbmin[1]);
  printf("(%d, %d)\n", bbmax[0], bbmax[1]);
#endif
  return {bbmin, bbmax};
}

static Vec3f GetBarycentric(Vec2f a, Vec2f b, Vec2f c, Vec2f p) noexcept
{
  auto pa = a - p;
  auto ab = b - a;
  auto ac = c - a;

  Vec3f u(ac.x(), ab.x(), pa.x());
  Vec3f v(ac.y(), ab.y(), pa.y());
  
  auto t = CrossProduct3(v, u);
  if (std::fabs(t.z()) <= 1e-2)
    return { -1., 1., 1. };
  else
    return Vec3f(1. - (t.x()+t.y())/t.z(), t.y()/t.z(), t.x()/t.z());
}

void DrawTriangle(Vec2i a, Vec2i b, Vec2i c, FrameColor const &color,
                  FrameBuffer &buffer) noexcept
{
#if 0
  Vec3f bc_coor;

  Vec2i bbmin, bbmax;
  std::tie(bbmin, bbmax) = GetBoundingBox(a, b, c);

  for (int i = bbmin.x(); i <= bbmax.x(); ++i) {
    for (int j = bbmin.y(); j <= bbmax.y(); ++j) {
      bc_coor = GetBarycentric(a, b, c, {i, j});
#if 0
      printf("barycentric coordinate: (%.3f, %.3f, %.3f)\n", bc_coor.x(),
             bc_coor.y(), bc_coor.z());
#endif
      if (bc_coor.x() < 0 || bc_coor.y() < 0 || bc_coor.z() < 0) {
        continue;
      }

      buffer.SetPixel(i, j, color);
    }
  }
#endif
}

float WorldToScreenCoor(int axis, float world_coor) noexcept
{
  return ((world_coor + 1.0) * axis / 2);
}

void DrawTriangle(std::array<FragmentContext, 3> const &fctxs,
                  ShaderInterface *shader, FrameBuffer &buffer) noexcept
{
  Vec3f bc_coor;
  Vec2f bbmin, bbmax;
  
  Vec3f world_coors[3];
  
  for (int i = 0; i < 3; ++i) {
    world_coors[i] = fctxs[i].world_pos;
  }

  Vec3f ndc_coors[3];
  for (int i = 0; i < 3; ++i) {
    DebugPrintf("World Coordinate = (%f, %f, %f)\n", world_coors[i][0], world_coors[i][1], world_coors[i][2]);
    DebugPrintf("Clip Coordinate = (%f, %f, %f, %f)\n", fctxs[i].clip_pos[0], fctxs[i].clip_pos[1], fctxs[i].clip_pos[2], fctxs[i].clip_pos[3]);
    ndc_coors[i] = ClipVec<3>(fctxs[i].clip_pos/fctxs[i].clip_pos[3]);
    
    DebugPrintf("NDC Coordinate = (%f, %f, %f, %f)\n", ndc_coors[i][0], ndc_coors[i][1], ndc_coors[i][2]);
    if (std::fabs(ndc_coors[i][0]) > 1.0 || std::fabs(ndc_coors[i][1]) > 1.0)
      return;
  }
  
  Vec2f screen_coor[3];
  float screen_depth[3];

  for (int i = 0; i < 3; ++i) {
    screen_coor[i][0] = (ndc_coors[i].x() + 1.0) * buffer.GetWidth() / 2;
    screen_coor[i][1] = (ndc_coors[i].y() + 1.0) * buffer.GetHeight() / 2;
    screen_depth[i] = ndc_coors[i].z();
  }
  
  Vec2f clamp(buffer.GetWidth()-1, buffer.GetHeight()-1);
  std::tie(bbmin, bbmax) =
      GetBoundingBox(screen_coor, clamp);

  Vec2i p;
  for (p[0] = bbmin.x(); p[0] <= bbmax.x(); p[0]++) {
    for (p[1] = bbmin.y(); p[1] <= bbmax.y(); p[1]++) {
      bc_coor = GetBarycentric(screen_coor[0], screen_coor[1], screen_coor[2],
                               ToVecf(p));
      if (bc_coor.x() < 0 || bc_coor.y() < 0 || bc_coor.z() < 0) {
        continue;
      }

      float interpolated_depth = 0.;
      for (int i = 0; i < 3; ++i) {
        interpolated_depth += bc_coor[i] * screen_depth[i];
      }

      if (interpolated_depth <= buffer.GetDepth(p.x(), p.y())) 
      {
        continue;
      }

      FrameColor color;
      FragmentContext fctx;

      // 注意叉积方向
      auto face_normal =
          CrossProduct3(world_coors[1] - world_coors[0], world_coors[2] - world_coors[0])
              .Normalize();

      fctx.intensity =
          DotProduct(face_normal, shader->uniform_light_dir);

      if (shader->FragmentProcess(fctx, color)) {
        buffer.UpdateDepth(p.x(), p.y(), interpolated_depth);
        buffer.SetPixel(p.x(), p.y(), color);
      }
    }
  }
}

} // namespace kuro
