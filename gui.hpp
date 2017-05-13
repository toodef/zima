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
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>
#include <QHeaderView>

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
   void file_info();
   void threshold();

private:
   void init_menu();
      void closeEvent(QCloseEvent * event) override;

   std::shared_ptr<renderer_t> renderer_;

   std::shared_ptr<QMenu> file_menu_, view_menu_, about_menu_;
   std::shared_ptr<QAction> open_file_, file_info_, threshold_;
   std::shared_ptr<QFileDialog> file_dialog_;
   std::shared_ptr<QWidget> thresh_window_;
   std::shared_ptr<QTableWidget> file_info_window_;

   std::shared_ptr<image_t> image_;
};