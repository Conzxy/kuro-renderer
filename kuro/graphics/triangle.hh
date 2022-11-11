#ifndef KURO_GRAPHICS_TRIANGLE_H__
#define KURO_GRAPHICS_TRIANGLE_H__

#include "kuro/img/frame_buffer.hh"
#include "kuro/math/vec.hh"

namespace kuro {

class ShaderInterface;
class FragmentContext;

void DrawTriangle(Vec2i a, Vec2i b, Vec2i c, FrameColor const &color,
                  FrameBuffer &buffer) noexcept;
void DrawTriangle(std::array<FragmentContext, 3> const& fctxs,
                  ShaderInterface *shader, FrameBuffer &buffer) noexcept;

} // namespace kuro

#endif
