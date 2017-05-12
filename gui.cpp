#include <QtWidgets/QVBoxLayout>
#include "gui.hpp"

gui_t::gui_t( QWidget * parent ) : QMainWindow(parent)
{
   setWindowTitle("Tif Viewer");

   resize(1000, 600);

   QWidget * central_widget = new QWidget;
   QVBoxLayout * central_layout = new QVBoxLayout;

   renderer_ = new renderer_t();
   central_layout->addWidget(renderer_);

   central_widget->setLayout(central_layout);
   setCentralWidget(central_widget);

}

gui_t::~gui_t()
{

}
