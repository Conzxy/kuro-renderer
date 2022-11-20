#ifndef KURO_GUI_MAIN_WINDOW_H__
#define KURO_GUI_MAIN_WINDOW_H__

#include <QMainWindow>
#include <QTimer>
#include <unordered_map>

#include "kuro/img/model.hh"
#include "renderer_view.hh"

class QAction;
class QMenu;

namespace kuro {

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  MainWindow();
  
  void Start();
 private:
  void CreateActions(); 
  void CreateMenus();
  
  QMenu *file_menu_;

  QAction *load_action_;

  QTimer title_timer_;


  RendererView *renderer_view_;
};

} // namespace kuro

#endif
