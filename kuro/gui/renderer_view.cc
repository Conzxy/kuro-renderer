#include "renderer_view.hh"

#include <QImage>
#include <QKeyEvent>
#include <QTimer>
#include <QWidget>
#include <QDebug>

#include "graphics/camera.hh"
#include "graphics/line.hh"
#include "graphics/transform.hh"
#include "graphics/triangle.hh"
#include "img/frame_buffer.hh"
#include "img/model.hh"
#include "img/qimage_util.hh"

#include "graphics/flat_shader.hh"

using namespace kuro;

#define WIDTH 800
#define HEIGHT 800

static Vec2i QPointToVec(QPoint const &p)
{
  return { p.x(), p.y() };
}


RendererView::RendererView()
  : scene_(new QGraphicsScene())
  , px_item_(new QGraphicsPixmapItem())
  , shader(new FlatShader())
  , frame_buffer(WIDTH, HEIGHT, FrameBuffer::IMAGE_TYPE_RGB)
{
  scene_->addItem(px_item_);
  scene_->setBackgroundBrush(Qt::black);

  // flip vertically
  scale(1, -1);
  setScene(scene_);

  timer_ = new QTimer(this);

  connect(timer_, &QTimer::timeout, this, [this]() {
    Render();
  });
}

RendererView::~RendererView() noexcept
{
  delete scene_;
  delete shader;
  // The px_item_ ownership is transfered to scene_
}

void RendererView::SetImage(QImage const &image) { px_item_->setPixmap(QPixmap::fromImage(image)); }

void RendererView::SetModel(Model &model)
{
  model_ = &model;
  ResetCamera();
}

void RendererView::StartRender()
{
  timer_->start(1);
  fps_time_.restart();

  Render();
}

void RendererView::StopRender()
{
  timer_->stop();
  fps_time_.invalidate();

  frame_context_.avg_time = 0;
  frame_context_.fps = 0;
  frame_count_ = 0;
}

void RendererView::mousePressEvent(QMouseEvent *e)
{
  Vec2i cur_pos = QPointToVec(e->pos());
  auto buttons = e->buttons();
  if (buttons & Qt::LeftButton) {
    camera_ctx_.is_orbiting = true;
    camera_ctx_.pressd_pos = cur_pos;
    camera_ctx_.orbit_end = cur_pos;
  } else if (buttons & Qt::RightButton) {
    camera_ctx_.is_paning = true;
    camera_ctx_.pressd_pos = cur_pos;
    camera_ctx_.pan_end = cur_pos;
  }
}

void RendererView::mouseMoveEvent(QMouseEvent *e)
{
  Vec2i cur_pos = QPointToVec(e->pos());
  if (camera_ctx_.is_orbiting) {
    camera_ctx_.orbit_end = cur_pos;
  } 
  if (camera_ctx_.is_paning) {
    camera_ctx_.pan_end = cur_pos;
  }
}

void RendererView::mouseReleaseEvent(QMouseEvent *e)
{
  Vec2i cur_pos = QPointToVec(e->pos());
  if (e->button() == Qt::LeftButton && camera_ctx_.is_orbiting) {
    camera_ctx_.is_orbiting = false;
    camera_ctx_.orbit_end = cur_pos;
  }
  if (e->button() == Qt::RightButton && camera_ctx_.is_paning) {
    camera_ctx_.is_paning = false;
    camera_ctx_.pan_end = cur_pos;
  }
}

void RendererView::keyPressEvent(QKeyEvent *ev)
{
  if (ev->key() == Qt::Key_Escape) {
    this->close();
  } else {
    QGraphicsView::keyPressEvent(ev);
  }
}

void RendererView::wheelEvent(QWheelEvent *e)
{
  // backwards to user --> negative
  // --> distance increase --> smaller
  //
  // reverse this behavior
  auto angle_delta = e->angleDelta().y();
  camera_ctx_.dolly_exp = -float(angle_delta) / 120;
  
  e->accept();
}

#if 1
void RendererView::paintEvent(QPaintEvent *e)
{
  QGraphicsView::paintEvent(e);
  // QPainter painter(this);
  // painter.drawImage(0, 0, image);
}
#endif

void RendererView::ResetCamera() noexcept
{
  new (&camera_ctx_) CameraContext();
}

void RendererView::Render()
{
  static Camera camera({0., 0., 0.}, {0., 0., 1.},
                       frame_buffer.GetWidth() / (float)frame_buffer.GetHeight());

  frame_buffer.ClearAllPixel();
  frame_buffer.ClearDepth();

  camera_ctx_.height = height();
  camera_ctx_.width = width();

  camera.Update(camera_ctx_);
  shader->varying_projection_matrix = camera.GetProjectionMatrix();
  shader->varying_view_matrix = camera.GetViewMatrix();

  shader->varying_viewport_matrix = GetViewportMatrix(frame_buffer.GetWidth(), frame_buffer.GetHeight());
  shader->varying_model_matrix = GetIdentityF<4>();

  shader->uniform_light_dir = {0, 0, 1};
  shader->uniform_light_dir = ClipVec<3>(shader->varying_view_matrix * EmbedVec<4, 3, float>(shader->uniform_light_dir, 0));

  for (size_t i = 0; i < model_->GetFacesNum(); ++i) {
    auto &face = model_->GetFace(i);
    std::array<FragmentContext, 3> fctxs;
    for (size_t j = 0; j < face.size(); j += 3) {
      for (int k = 0; k < 3; ++k) {
        auto &mesh = face[k + j];
        VertexContext vctx{
            .pos = model_->GetVertex(mesh.vertex_idx),
            .normal = model_->GetNormal(mesh.normal_idx),
            .uv = ClipVec<2>(model_->GetTexture(mesh.uv_idx)),
        };
        fctxs[k] = shader->VertexProcess(vctx);
      }

      DrawTriangle(fctxs, shader, frame_buffer);
    }
  }

  image = FrameBufferToQImage(frame_buffer);
  px_item_->setPixmap(QPixmap::fromImage(image));

  if (frame_count_ == 0) {
    fps_time_.start();
  } else {
    auto const elapsed = fps_time_.elapsed();
    frame_context_.avg_time =  elapsed / frame_count_;
    frame_context_.fps = frame_count_ / (float(elapsed) / 1000);
  }

  frame_count_++;
}
