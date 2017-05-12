#pragma once

#include <QMainWindow>
#include <QGLWidget>

#include <cstdint>

#include "renderer/renderer.hpp"

class gui_t: public QMainWindow
{
   Q_OBJECT

public:
   gui_t( QWidget * parent = 0 );
   virtual ~gui_t();

private:
   renderer_t * renderer_;

   QGLWidget * gl_widget_;
};