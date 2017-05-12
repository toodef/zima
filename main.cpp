#include <QApplication>

#include "gui.hpp"

//C:\Qt\5.8\msvc2015_64\lib

int main( int argc, char ** argv )
{
   QApplication app(argc, argv);
   app.setAttribute( Qt::AA_UseDesktopOpenGL );

   gui_t window;

   window.show();

   return app.exec();
}
