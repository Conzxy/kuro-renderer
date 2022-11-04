#include <QApplication>

#include "gui/renderer_view.hh"
#include "img/frame_buffer.hh"
#include "img/qimage_util.hh"
#include "graphics/line.hh"

using namespace kuro;

int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  RendererView renderer;

  FrameBuffer frame_buffer(100, 100, FrameBuffer::IMAGE_TYPE_RGB);
  DrawLine3({10, 20}, {30, 90}, FrameColor::red, frame_buffer);
  DrawLine3({30, 90}, {10, 20}, FrameColor::red, frame_buffer);
  DrawLine3({13, 20}, {80, 40}, FrameColor::white, frame_buffer);
  DrawLine3({10, 20}, {10, 90}, FrameColor::white, frame_buffer);
  DrawLine3({10, 20}, {90, 20}, FrameColor::white, frame_buffer);
  DrawLine3({80, 20}, {20, 60}, FrameColor::red, frame_buffer);

  QImage image = FrameBufferToQImage(frame_buffer);
  renderer.SetImage(image);
  renderer.show();

  return app.exec();
}
