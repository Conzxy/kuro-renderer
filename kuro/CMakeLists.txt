QT5_WRAP_CPP(MOC_FILES
  gui/renderer_view.hh
)

file(GLOB_RECURSE KURO_SRC
  util/*.cc
  img/*.cc
  graphics/*.cc
  gui/*.cc
  math/*.cc
)

list(APPEND KURO_SRC ${MOC_FILES})

GenLib(${KURO_LIB} ${KURO_SRC})

set(RENDERER_LIBS ${KURO_LIB} Qt5::Gui Qt5::Widgets)
set(RENDERER_SRC main.cc)

GenApplication(renderer RENDERER_SRC RENDERER_LIBS)
