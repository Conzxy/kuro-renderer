#include "rasterizer.hh"

#include "triangle.hh"

using namespace kuro;

Rasterizer::Rasterizer(Model *model, ShaderInterface *shader)
  : model_(model)
  , shader_(shader)
{
}

void Rasterizer::Render(FrameBuffer &frame_buffer)
{
  frame_buffer.ClearAllPixel();
  frame_buffer.ClearDepth();

  for (size_t i = 0; i < model_->GetFacesNum(); ++i) {
    auto &face = model_->GetFace(i);
    std::array<FragmentContext, 3> fctxs;
    const auto polygon_vertex_num = face.size();
    if (polygon_vertex_num < 3 || polygon_vertex_num > 4) {
      fprintf(stderr, "Can't process polygon whose vertexes num less than 3 or "
                      "greater than 4");
      return;
    }

    int tri_vtxes[3] = {0, 1, 2};

    int tri_num = 1;
    if (polygon_vertex_num == 4) tri_num++;
    for (; tri_num > 0; --tri_num) {
      for (int i = 0; i < 3; ++i) {
        auto &mesh = face[tri_vtxes[i]];
        VertexContext vctx{
            .pos = model_->GetVertex(mesh.vertex_idx),
            .normal = model_->GetNormal(mesh.normal_idx),
            .uv = ClipVec<2>(model_->GetTexture(mesh.uv_idx)),
        };
        fctxs[i] = shader_->VertexProcess(vctx);
      }

      DrawTriangle(fctxs, shader_, frame_buffer);

      if (polygon_vertex_num == 4) {
        tri_vtxes[0] = 2;
        tri_vtxes[1] = 3;
        tri_vtxes[2] = 0;
      }
    }
  }
}
