#include <QApplication>

#include "gui.hpp"

int main( int argc, char ** argv )
{
   QApplication app(argc, argv);

   gui_t window;

   window.show();

   app.exec();
}
