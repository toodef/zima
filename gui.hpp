#pragma once

#include <QtWidgets/QtWidgets>

class gui_t: public QMainWindow
{
   Q_OBJECT

public:
   gui_t( QWidget * parent = 0 );
   virtual ~gui_t();
};