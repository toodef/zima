#include "gui.hpp"

gui_t::gui_t( QWidget * parent ) : QMainWindow(parent)

{
   resize(1000, 600);

   graphics_view_ = new QGraphicsView(this);
   QGLWidget * viewport = new QGLWidget(QGLFormat(QGL::SampleBuffers));
   graphics_view_->setViewport(viewport);
   graphics_view_->setRenderHint(QPainter::Antialiasing, false);

   this->setCentralWidget(graphics_view_);
}

gui_t::~gui_t()
{

}
