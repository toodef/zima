#pragma once

#include <QMainWindow>
#include <QGLWidget>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QSlider>

#include <cstdint>

#include "renderer/renderer.hpp"

class gui_t: public QMainWindow
{
   Q_OBJECT

public:
   explicit gui_t( QWidget * parent = 0 );
   ~gui_t() override;

private slots:
   void open_file();
   void threshold();

private:
   void init_menu();
      void closeEvent(QCloseEvent * event) override;

   renderer_t * renderer_;

   QMenu * file_menu_, * view_menu_, * about_menu_;

   QAction * open_file_, * threshold_;

   QFileDialog * file_dialog_;

   QWidget * thresh_window_;
};