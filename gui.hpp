#pragma once

#include <QMainWindow>
#include <QtOpenGL/QtOpenGL>
#include <QGraphicsView>
#include <QFrame>

#include <cstdint>

#include "renderer/renderer.hpp"

class gui_t: public QMainWindow
{
   Q_OBJECT

public:
   gui_t( QWidget * parent = 0 );
   virtual ~gui_t();

private:
   //renderer_t renderer_;

   QGraphicsView * graphics_view_;
};