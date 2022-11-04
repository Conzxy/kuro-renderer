#include "line.hh"

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "kuro/img/tga_image.hh"
#include "kuro/img/frame_buffer.hh"

namespace kuro {

void DrawLine0(Point2D const &p0, Point2D const &p1, TgaImage &image, TgaColor const &color)
{
  for (float i = 0; i <= 1.0; i += 0.01) {
    image.SetPixel(p0.x+(p1.x-p0.x)*i, p0.y+(p1.y-p0.x) * i, color);
  }
}

void DrawLine1(Point2D const &p0, Point2D const &p1, TgaImage &image, TgaColor const &color)
{

}

void DrawLine0(int x0, int y0, int x1, int y1, FrameColor const &c, FrameBuffer &frame)
{
  // Depend on the constant:
  // TO this, may 0.01 is enough,
  // but to other, 0.01 is dense.
  for (float i = 0; i < 1.; i += .01) {
    frame.SetPixel(x0+(x1-x0)*i, y0+(y1-y0)*i, c);
  }
}

void DrawLine1(Point2D p0, Point2D p1, FrameColor const &c, FrameBuffer &frame_buffer)
{
  float k = 0;
  // 1. depend on the order between p0 and p1
  // If p1.x > p0.x, do nothing
  //
  // 2. If the slope greater than 1,
  //    there are some holes between dot

  for (int x = p0.x; x < p1.x; ++x) {
    k = float(x - p0.x) / (p1.x - p0.x);
    frame_buffer.SetPixel(x, p0.y + (p1.y-p0.y)*k, c);
  }
}

void DrawLine2(Point2D p0, Point2D p1, FrameColor const &c, FrameBuffer &frame_buffer)
{
  // slope > 1
  bool steep = false;

  // Make slope less than 1
  if (std::abs(p1.x - p0.x) < std::abs(p1.y - p0.y)) {
    std::swap(p0.x, p0.y);
    std::swap(p1.x, p1.y);
    steep = true;
  }
  
  // Reorder points
  if (p0.x > p1.x) {
    kuro::swap(p0, p1);
  }
  
  float k;
  int y = 0;
  
  for (int x = p0.x; x <= p1.x; ++x) {
    // Hot spot
    // Performance wrong!
    k = (x-p0.x)/(float)(p1.x-p0.x);
    y = (p1.y - p0.y) * k + p0.y;
    if (steep)
      frame_buffer.SetPixel(y, x, c);
    else
      frame_buffer.SetPixel(x, y, c);
  }
}

/**
 * Bresenham's line drawing algorithm in float arithmetic(slope) version
 */
void DrawLine3(Point2D p0, Point2D p1, FrameColor const &c, FrameBuffer &frame_buffer)
{
  assert(p0.x >= 0 && p0.y >= 0 && p1.x >= 0 && p1.y >= 0 &&
         "The coordinates must be >= 0");

  // error:
  // The distance between top of the pixel and the y coordinate of next point
  // 
  // Suppose t is the slope of line,
  // The initialized value of error is t - 1, then
  // add m when x increments 1.
  // If error > 0, indicates pixel advanced(increments y).
  //
  // Precondition:
  // t < 1, i.e. delta(x) > delta(y)

  // slope > 1
  bool steep = false;

  // Make slope less than 1
  if (std::abs(p1.x - p0.x) < std::abs(p1.y - p0.y)) {
    std::swap(p0.x, p0.y);
    std::swap(p1.x, p1.y);
    steep = true;
  }
  
  // Reorder points
  if (p0.x > p1.x) {
    kuro::swap(p0, p1);
  }
  
  // Decrease the count of float division to 1
  float k = std::fabs(float(p1.y-p0.y) / (p1.x - p0.x));

  assert(k <= 1 && "The absolute slope must be <= 1");

  float error = k - 1;
  int y = p0.y;

  assert(p0.x < p1.x && "The first point must be left");
  for (int x = p0.x; x < p1.x; ++x) {
    if (steep)
      frame_buffer.SetPixel(y, x, c);
    else
      frame_buffer.SetPixel(x, y, c);
    error += k;
    if (error > 0) {
      error -= 1;
      assert(p1.y != p0.y);
      y += (p1.y > p0.y) ? 1 : -1;
    }
  }
}

/**
 * Bresenham's line drawing algorithm in integer arithmetic version
 *
 * Compared to previous slope version, use integer to instead float division
 */
void DrawLine(Point2D p0, Point2D p1, FrameColor const &c, FrameBuffer &frame_buffer)
{
  assert(p0.x >= 0 && p0.y >= 0 && p1.x >= 0 && p1.y >= 0 &&
         "The coordinates must be >= 0");

  // error:
  // The distance between top of the pixel and the y coordinate of next point
  // 
  // Suppose t is the slope of line,
  // The initialized value of error is t - 1, then
  // add m when x increments 1.
  // If error > 0, indicates pixel advanced(increments y).
  //
  // Precondition:
  // t < 1, i.e. delta(x) > delta(y)

  // slope > 1
  bool steep = false;

  // Make slope less than 1
  if (std::abs(p1.x - p0.x) < std::abs(p1.y - p0.y)) {
    std::swap(p0.x, p0.y);
    std::swap(p1.x, p1.y);
    steep = true;
  }
  
  // Reorder points
  if (p0.x > p1.x) {
    kuro::swap(p0, p1);
  }
  
  int delta_x = std::abs(p1.x - p0.x);
  int delta_y = std::abs(p1.y - p0.y);

  float k = float(delta_y) / (delta_x);
  assert(k >= 0 && k <= 1 && "The absolute slope must be <= 1");
  
  float error = delta_y - delta_x;
  int y = p0.y;

  assert(p0.x < p1.x && "The first point must be left");
  for (int x = p0.x; x < p1.x; ++x) {
    if (steep)
      frame_buffer.SetPixel(y, x, c);
    else
      frame_buffer.SetPixel(x, y, c);
    error += delta_y;
    if (error > 0) {
      error -= delta_x;
      // If p1.y == p0.y,
      // the delta_y == 0,
      // error <= 0 forever.
      assert(p1.y != p0.y);
      y += (p1.y > p0.y) ? 1 : -1;
    }
  }
  
}

} // namespace kuro
