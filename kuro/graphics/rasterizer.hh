#ifndef KURO_RASTERIZER_H__
#define KURO_RASTERIZER_H__

#include "kuro/util/noncopyable.hh"
#include "kuro/img/model.hh"
#include "kuro/img/frame_buffer.hh"

#include "shader_interface.hh"

namespace kuro {

class Rasterizer : public kanon::noncopyable {
 public:
  Rasterizer() = default;
  Rasterizer(Model *model, ShaderInterface *shader);
  
  ~Rasterizer() noexcept = default;

  void Render(FrameBuffer &frame_buffer);

  Model *GetModel() noexcept { return model_; }
  Model const *GetModel() const noexcept { return model_; }
  void SetModel(Model *model) noexcept { model_ = model; }
  
  ShaderInterface const *GetShader() const noexcept { return shader_; }
  ShaderInterface *GetShader() noexcept { return shader_; }
  void SetShader(ShaderInterface *shader) noexcept { shader_ = shader; }

 private:
  Model *model_ = nullptr;
  ShaderInterface *shader_ = nullptr;
};

} // namespace kuro

#endif
