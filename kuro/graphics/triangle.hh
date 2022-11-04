#ifndef KURO_GRAPHICS_TRIANGLE_H__
#define KURO_GRAPHICS_TRIANGLE_H__

#include "kuro/img/frame_buffer.hh"
#include "kuro/math/vec.hh"

namespace kuro {

void DrawTriangle(Vec2i a, Vec2i b, Vec2i c, FrameColor const &color, FrameBuffer &buffer) noexcept;

} // namespace kuro

#endif
