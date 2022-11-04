#ifndef KURO_GRAPHICS_LINE_H__
#define KURO_GRAPHICS_LINE_H__

#include <utility>

namespace kuro {

class TgaImage;
class TgaColor;
class FrameBuffer;
class FrameColor;

struct Point2D  {
  int x;
  int y;
};

inline void swap(Point2D &lhs, Point2D &rhs) noexcept
{
  std::swap(lhs.x, rhs.x);
  std::swap(lhs.y, rhs.y);
}

void DrawLine0(Point2D const &p0, Point2D const &p1, TgaImage &image, TgaColor const &color);
void DrawLine1(Point2D const &p0, Point2D const &p1, TgaImage &image, TgaColor const &color);

void DrawLine0(int x0, int y0, int x1, int y1, FrameColor const &c, FrameBuffer &frame);
void DrawLine1(Point2D p0, Point2D p1, FrameColor const &c, FrameBuffer &frame_buffer);
void DrawLine2(Point2D p0, Point2D p1, FrameColor const &c, FrameBuffer &frame_buffer);
void DrawLine3(Point2D p0, Point2D p1, FrameColor const &c, FrameBuffer &frame_buffer);

void DrawLine(Point2D p0, Point2D p1, FrameColor const &c, FrameBuffer &frame_buffer);

} // namespace kuro

#endif // KURO_GRAPHICS_LINE_H__
