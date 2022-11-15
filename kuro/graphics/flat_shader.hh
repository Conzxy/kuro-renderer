#ifndef KURO_GRAPHICS_FLAT_SHADER_H__
#define KURO_GRAPHICS_FLAT_SHADER_H__

#include "shader_interface.hh"
#include "kuro/img/frame_buffer.hh"
#include "kuro/util/log.hh"

namespace kuro {

class FlatShader : public ShaderInterface {
 public:
  FlatShader() = default;
  ~FlatShader() override = default;

  FragmentContext VertexProcess(VertexContext &vctx) override
  {
    FragmentContext fctx;
    fctx.world_pos = vctx.pos;
  
    // auto mvp = varying_projection_matrix * varying_view_matrix;
    // auto mvp = varying_view_matrix; 
    fctx.clip_pos = varying_model_matrix * EmbedVecf<4>(vctx.pos, 1);
    fctx.clip_pos = varying_view_matrix * fctx.clip_pos;
    fctx.clip_pos[2] -= 1.1;
    fctx.clip_pos = varying_projection_matrix * fctx.clip_pos;
    
    fctx.uv = vctx.uv;
    fctx.world_normal = vctx.normal;

    return fctx;
  }

  bool FragmentProcess(FragmentContext &fctx, FrameColor &color) override
  {
    if (fctx.intensity < 0) return false;

    color.r = fctx.intensity * 255;
    color.g = fctx.intensity * 255;
    color.b = fctx.intensity * 255;
    return true;
  }

 private:
};

} // namespace kuro

#endif
