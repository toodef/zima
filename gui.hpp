#pragma once

#include <QtWidgets/QtWidgets>
#include <QtCore/QCoreApplication>
//#include <QtCore/QCommandLineParser>
//#include <QtCore/QCommandLineOption>

class gui_t: public QMainWindow
{
   Q_OBJECT

public:
   gui_t( QWidget * parent = 0 );
   virtual ~gui_t();
};