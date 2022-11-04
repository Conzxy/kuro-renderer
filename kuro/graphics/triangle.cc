#include "triangle.hh"
#include <climits>

namespace kuro {

static Vec3f GetBarycentric(Vec2i a, Vec2i b, Vec2i c, Vec2i p) noexcept
{
  Vec2i pa = a - p;
  Vec2i ab = b - a;
  Vec2i ac = c - a;

  Vec3i u(pa.x(), ab.x(), ac.x());
  Vec3i v(pa.y(), ab.y(), ac.y());
  auto cross_result = CrossProduct3(u, v);

  if (cross_result.x() == 0)
    return {-1., 1., 1.};
  else
    return Vec3f(1. - float(cross_result.y() + cross_result.z()) /
                          cross_result.x(),
                 float(cross_result.y()) / cross_result.x(),
                 float(cross_result.z()) / cross_result.x());
}

void DrawTriangle(Vec2i a, Vec2i b, Vec2i c, FrameColor const &color,
                  FrameBuffer &buffer) noexcept
{
  Vec2i bbmin(a.x(), a.y());
  Vec2i bbmax(a.x(), a.y());

  int min_x;
  int max_x;
  int min_y;
  int max_y;

  if (b.x() < c.x()) {
    min_x = b.x();
    max_x = c.x();
  } else {
    min_x = c.x();
    max_x = b.x();
  }

  if (b.y() < c.y()) {
    min_y = b.y();
    max_y = c.y();
  } else {
    min_y = c.y();
    max_y = b.y();
  }

  bbmin[0] = std::min(bbmin[0], min_x);
  bbmin[1] = std::min(bbmin[1], min_y);
  bbmax[0] = std::max(bbmax[0], max_x);
  bbmax[1] = std::max(bbmax[1], max_y);

#if 0
  printf("Bounding box: \n");
  printf("(%d, %d)\n", bbmin[0], bbmin[1]);
  printf("(%d, %d)\n", bbmax[0], bbmax[1]);
#endif

  Vec3f bc_coor;
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
}

} // namespace kuro
