#ifndef KURO_GUI_RENDERER_VIEW_H__
#define KURO_GUI_RENDERER_VIEW_H__

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QTime>
#include <QTimer>
#include <QElapsedTimer>

#include "kuro/graphics/camera.hh"
#include "kuro/img/frame_buffer.hh"

namespace kuro {

class Model;
class ShaderInterface;

struct FrameContext {
  float avg_time = 0;
  float fps = 0;
};

class RendererView : public QGraphicsView {
  Q_OBJECT
 public:
  RendererView();
  ~RendererView() noexcept override;
  
  void SetImage(QImage const &image);
  void SetModel(Model &model);

  void StartRender();
  void StopRender();
  
  void ResetCamera() noexcept;

  FrameContext GetFrameContext() const noexcept
  {
    return frame_context_;
  }

 protected:
  void mouseMoveEvent(QMouseEvent *e) override;
  void mousePressEvent(QMouseEvent *e) override;
  void mouseReleaseEvent(QMouseEvent *e) override;
  void wheelEvent(QWheelEvent *e) override;

 private:
  void Render();

  QGraphicsScene *scene_;
  QGraphicsPixmapItem *px_item_;
  
  QTimer timer_;
  QElapsedTimer fps_time_;

  QImage image;

  Model *model_ = nullptr;
  ShaderInterface *shader = nullptr;
  FrameBuffer frame_buffer;
  
  float frame_count_ = 0;
  FrameContext frame_context_;
  
  Vec3f init_position_;
  Vec3f init_target_;
  Camera camera_;
  CameraContext camera_ctx_;
};

} // namespace kuro

#endif
