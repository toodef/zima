#pragma once

#include <QtWidgets/QMainWindow>
#include <QGLWidget>

#include <iostream>

class renderer_t : public QGLWidget
{
   Q_OBJECT

public:
   renderer_t( QWidget * parent = 0 );

   void * display();
private:
   QOpenGLContext * gl_context_;
};