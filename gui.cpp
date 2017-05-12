#include <QtWidgets/QVBoxLayout>
#include "gui.hpp"

gui_t::gui_t( QWidget * parent ) : QMainWindow(parent)
{
   setWindowTitle("Tif Viewer");

   resize(1000, 600);

   QWidget * central_widget = new QWidget;
   QVBoxLayout * central_layout = new QVBoxLayout;

   gl_widget_ = new QGLWidget();
   central_layout->addWidget(gl_widget_);

   central_widget->setLayout(central_layout);
   setCentralWidget(central_widget);

   renderer_ = new renderer_t(this);
}

gui_t::~gui_t()
{

}
