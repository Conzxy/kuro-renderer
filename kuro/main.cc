#include <QApplication>

#include <float.h>
#include <thread>

#include "gui/renderer_view.hh"
#include "graphics/line.hh"
#include "graphics/triangle.hh"
#include "graphics/flat_shader.hh"

#include "img/frame_buffer.hh"
#include "img/model.hh"
#include "img/qimage_util.hh"
#include "setting/option.hh"

#include "gui/main_window.hh"

#include <third-party/takina/takina.h>

using namespace kuro;

int main(int argc, char **argv)
{
  char usage[4096];
  snprintf(usage, sizeof usage, "%s [-m|--model obj files...]", argv[0]);

  takina::AddUsage(usage);
  takina::AddDescription("Simple software renderer don't depend on OpenGL\n"
      "");
  
  auto &opt = kuro_option();
  takina::AddOption({"m", "model", "Wavefront object files", "MODEL FILES"}, &opt.models);
  
  std::string errmsg;
  if (!takina::Parse(argc, argv, &errmsg)) {
    fprintf(stderr, "%s", errmsg.c_str());
    return 1;
  }

  QApplication app(argc, argv);
  MainWindow mwin;

  mwin.Start();

  return app.exec();
}
