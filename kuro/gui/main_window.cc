#include "main_window.hh"

#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>

#include "kuro/util/qstring_util.hh"
#include "kuro/setting/option.hh"

using namespace kuro;

MainWindow::MainWindow()
  : renderer_view_(new RendererView)
{
  connect(&title_timer_, &QTimer::timeout, this, [this]() {
    char title[256];
    auto const frame_ctx = renderer_view_->GetFrameContext();
    snprintf(title, sizeof title, "Kuro Software Renderer | fps: %f average-time: %fms", 
        frame_ctx.fps, frame_ctx.avg_time);
    setWindowTitle(title);
  });
  
  title_timer_.start(1000);

  setCentralWidget(renderer_view_);

  CreateActions();
  CreateMenus();
}

void MainWindow::CreateActions()
{
  load_action_ = new QAction(tr("&Load"), this);

  connect(load_action_, &QAction::triggered, this, [this]() {
    auto model_name = QFileDialog::getOpenFileName(this,
        tr("Open 3D model"), ".",
        tr("Wavefront model file(*.obj)"));
  
    if (model_name.isEmpty()) return;
    
    renderer_view_->AddModel(QStringToCString(model_name));
  });
}

void MainWindow::CreateMenus()
{
  file_menu_ = menuBar()->addMenu(tr("&File"));
  file_menu_->addAction(load_action_);
}

void MainWindow::Start()
{
  auto &opt = kuro_option();
  
  for (auto const &model : opt.models) {
    if (!model.empty()) {
      renderer_view_->AddModel(model.c_str());
    }

  }
  
  renderer_view_->StartRender();
  show();
}
