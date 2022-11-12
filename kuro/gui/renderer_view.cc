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
  , init_position_(0., 0., 1.)
  , init_target_(0., 0., 0.)
  , camera_(init_target_, init_position_,
           frame_buffer.GetWidth() / (float)frame_buffer.GetHeight())
{
  scene_->addItem(px_item_);
  scene_->setBackgroundBrush(Qt::black);

  // flip vertically
  scale(1, -1);
  setScene(scene_);

  connect(&timer_, &QTimer::timeout, this, [this]() {
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

  frame_buffer.ClearAllPixel();
  frame_buffer.ClearDepth();

  camera_ctx_.height = height();
  camera_ctx_.width = width();

  camera_.Update(camera_ctx_);
  shader->varying_projection_matrix = camera_.GetProjectionMatrix();
  shader->varying_view_matrix = camera_.GetViewMatrix();

  shader->varying_viewport_matrix = GetViewportMatrix(frame_buffer.GetWidth(), frame_buffer.GetHeight());
  shader->varying_model_matrix = GetIdentityF<4>();

  shader->uniform_light_dir = {0, 0, 1};
  shader->uniform_light_dir = ClipVec<3>(shader->varying_view_matrix * EmbedVec<4, 3, float>(shader->uniform_light_dir, 0));

  for (size_t i = 0; i < model_->GetFacesNum(); ++i) {
    auto &face = model_->GetFace(i);
    std::array<FragmentContext, 3> fctxs;
    
    DebugPrintf("face vertexes count = %zu\n", face.size());
    if (face.size() < 3) break;
    const auto last_vtx_idx_after = face.size() - 2;
    for (size_t j = 0; j < last_vtx_idx_after; j++) {
      for (int k = 0; k < 3; ++k) {
        DebugPrintf("current vertex in face: %d\n", int(k+j));
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
