#ifndef KURO_GUI_RENDERER_VIEW_H__
#define KURO_GUI_RENDERER_VIEW_H__

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>

namespace kuro {

class RendererView : public QGraphicsView {
  Q_OBJECT
 public:
  RendererView();
  ~RendererView() noexcept override;
  
  void SetImage(QImage const &image);

 protected:
  void keyPressEvent(QKeyEvent *key_ev) override;
 private:
  QGraphicsScene *scene_;
  QGraphicsPixmapItem *px_item_;
};

} // namespace kuro

#endif
