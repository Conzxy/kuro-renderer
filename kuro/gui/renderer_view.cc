#include "renderer_view.hh"

#include <QImage>
#include <QKeyEvent>
#include <QTimer>
#include <QWidget>
#include <QDebug>
#include <stdio.h>

#include "kuro/graphics/camera.hh"
#include "kuro/graphics/line.hh"
#include "kuro/graphics/transform.hh"
#include "kuro/graphics/triangle.hh"
#include "kuro/img/frame_buffer.hh"
#include "kuro/img/model.hh"
#include "kuro/img/qimage_util.hh"
#include "kuro/graphics/flat_shader.hh"

using namespace kuro;

#define WIDTH 1000
#define HEIGHT 600

static Vec2i QPointToVec(QPoint const &p)
{
  return { p.x(), p.y() };
}

RendererView::RendererView()
  : scene_(new QGraphicsScene())
  , px_item_(new QGraphicsPixmapItem())
  , renderer_()
  , frame_buffer_(WIDTH, HEIGHT, FrameBuffer::IMAGE_TYPE_RGB)
  , init_position_(0., 0., 1.)
  , init_target_(0., 0., 0.)
  , camera_(init_target_, init_position_,
           frame_buffer_.GetWidth() / (float)frame_buffer_.GetHeight())
{
  scene_->addItem(px_item_);
  scene_->setBackgroundBrush(Qt::black);

  // flip vertically
  scale(1, -1);
  setScene(scene_);
  
  shader_ = new FlatShader();
  renderer_.SetShader(shader_);

  connect(&timer_, &QTimer::timeout, this, [this]() {
    Render();
  });
}

RendererView::~RendererView() noexcept
{
  delete scene_;
  delete shader_;
  // The px_item_ ownership is transfered to scene_
}

void RendererView::SetImage(QImage const &image) { px_item_->setPixmap(QPixmap::fromImage(image)); }

void RendererView::SetModel(Model &model)
{
  renderer_.SetModel(&model);
  ResetCamera();
}

void RendererView::StartRender()
{
  timer_.start(1);
  fps_time_.restart();

  Render();
}

void RendererView::StopRender()
{
  timer_.stop();
  fps_time_.invalidate();

  frame_context_.avg_time = 0;
  frame_context_.fps = 0;
  frame_count_ = 0;
}

void RendererView::mousePressEvent(QMouseEvent *e)
{
  Vec2i cur_pos = QPointToVec(e->pos());
  
  /*
   * In fact, press the left and right button,
   * the camera and target relative position hasn't changed
   */
  auto buttons = e->buttons();
  if (buttons & Qt::LeftButton) {
    camera_ctx_.is_orbiting = true;
    camera_ctx_.orbit_begin = cur_pos;
    camera_ctx_.orbit_end = cur_pos;
  }
  if (buttons & Qt::RightButton) {
    camera_ctx_.is_paning = true;
    camera_ctx_.pan_begin = cur_pos;
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

void RendererView::ResetCamera() noexcept
{
  camera_.SetPosition(init_position_);
  camera_.SetTarget(init_target_);
  camera_.ResetMotion();
  new (&camera_ctx_) CameraContext();
}

void RendererView::Render()
{
  camera_ctx_.height = height();
  camera_ctx_.width = width();

  camera_.Update(camera_ctx_);
  shader_->varying_projection_matrix = camera_.GetProjectionMatrix();
  shader_->varying_view_matrix = camera_.GetViewMatrix();
  shader_->varying_model_matrix = renderer_.GetModel()->GetModelMatrix();

  shader_->uniform_light_dir = {0, 0, 1};
  shader_->uniform_light_dir = ClipVec<3>(shader_->varying_view_matrix * EmbedVec<4, 3, float>(shader_->uniform_light_dir, 0));

  renderer_.Render(frame_buffer_);

  image = FrameBufferToQImage(frame_buffer_);
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
