#include <QApplication>

#include "gui/renderer_view.hh"
#include "img/frame_buffer.hh"
#include "img/qimage_util.hh"
#include "img/model.hh"
#include "graphics/line.hh"
#include "graphics/triangle.hh"

using namespace kuro;

int main(int argc, char **argv)
{
  if (2 > argc) {
    printf("Usage: %s *.obj\n", argv[0]);
    return 0;
  }

  QApplication app(argc, argv);

  RendererView renderer;

  FrameBuffer frame_buffer(500, 500, FrameBuffer::IMAGE_TYPE_RGB);
  Model model;

#if 1
  if (!model.ParseFrom(argv[1])) {
    fprintf(stderr, "Failed to parse *.obj file");
    return 0;
  }
  
  for (size_t i = 0; i < model.GetFacesNum(); ++i) {
    auto &face = model.GetFace(i);
    for (size_t j = 0; j < face.size(); j += 3) {
      auto v0 = model.GetVertex(face[j].vertex_idx);
      auto v1 = model.GetVertex(face[j+1].vertex_idx);
      auto v2 = model.GetVertex(face[j+2].vertex_idx);

      int x0 = (v0.x() + 1.) * frame_buffer.GetWidth() / 2.;
      int y0 = (v0.y() + 1.) * frame_buffer.GetHeight() / 2.;
      int x1 = (v1.x() + 1.) * frame_buffer.GetWidth() / 2.;
      int y1 = (v1.y() + 1.) * frame_buffer.GetHeight() / 2.;
      int x2 = (v2.x() + 1.) * frame_buffer.GetWidth() / 2;
      int y2 = (v2.y() + 1.) * frame_buffer.GetHeight() / 2;
      DrawTriangle({x0, y0}, {x1, y1}, {x2, y2},
               FrameColor(rand() % 255, rand() % 255, rand() % 255), frame_buffer);
    }
  }
#else
  DrawTriangle({ 100, 110 }, { 120, 300 }, { 150, 200 }, FrameColor::red, frame_buffer);
#endif

  QImage image = FrameBufferToQImage(frame_buffer);
  renderer.SetImage(image);
  renderer.show();

  return app.exec();
}
