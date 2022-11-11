#ifndef KURO_GRAPHICS_SHADER_INTERFACE_H__
#define KURO_GRAPHICS_SHADER_INTERFACE_H__

#include "kuro/math/vec.hh"
#include "kuro/math/matrix.hh"

#include "kuro/util/noncopyable.hh"

namespace kuro {

class FrameColor;

/*
 * 通过读取模型文件获取顶点信息
 */
struct VertexContext {
  Vec3f pos;
  Vec3f normal;
  Vec2f uv;
};

struct FragmentContext {
  Vec4f clip_pos;     // 剪切后的坐标
  Vec3f world_pos;    // 世界空间中的坐标
  Vec3f world_normal; // 世界空间的法向量
  Vec2f uv; // 像素的uv坐标
  float intensity; // 光照强度
};

class ShaderInterface : kanon::noncopyable {
 public:
  Vec3f uniform_light_dir;
  
  Matrix4x4f varying_model_matrix;
  Matrix4x4f varying_view_matrix;
  Matrix4x4f varying_projection_matrix;
  Matrix4x4f varying_viewport_matrix;
  
  ShaderInterface() = default;
  virtual ~ShaderInterface() = default;
  
  virtual FragmentContext VertexProcess(VertexContext &vctx) = 0;
  
  /**
   * \return
   *  false -- discard the fragment
   */
  virtual bool FragmentProcess(FragmentContext &fctx,
                               FrameColor &color) = 0;
};

} // namespace kuro

#endif
