#include <QtWidgets/QApplication>

#include "gui.hpp"

int main( int argc, char ** argv )
{
   QApplication app(argc, argv);
   app.setAttribute( Qt::AA_UseDesktopOpenGL );

   gui_t window(argc, argv);
   window.show();

   return app.exec();
}
