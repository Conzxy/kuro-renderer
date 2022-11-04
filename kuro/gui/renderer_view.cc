#include "renderer_view.hh"

#include <QImage>
#include <QKeyEvent>

using namespace kuro;

RendererView::RendererView()
  : scene_(new QGraphicsScene())
  , px_item_(new QGraphicsPixmapItem())
{
  scene_->addItem(px_item_);
  scene_->setBackgroundBrush(Qt::black);
  // flip vertically
  scale(1, -1);
  setScene(scene_);
}

RendererView::~RendererView() noexcept
{
  delete scene_;
  // The px_item_ ownership is transfered to scene_
}

void RendererView::SetImage(QImage const &image)
{
  px_item_->setPixmap(QPixmap::fromImage(image));
}

void RendererView::keyPressEvent(QKeyEvent *ev)
{
  if (ev->key() == Qt::Key_Escape) {
    this->close();
  } else {
    QGraphicsView::keyPressEvent(ev);
  }
}
