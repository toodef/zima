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
#include "gui_unit.hpp"

class gui_t: public QMainWindow
{
   Q_OBJECT

public:
   explicit gui_t( QWidget * parent = 0 );
   ~gui_t() override;

private slots:
   void open_file();

private:
   void init_menu();
      void closeEvent(QCloseEvent * event) override;

   std::shared_ptr<renderer_t> renderer_;

   std::shared_ptr<QMenu> file_menu_, view_menu_, about_menu_;
   std::shared_ptr<QAction> open_file_;
   std::shared_ptr<QFileDialog> file_dialog_;

   std::shared_ptr<gui_threshold_t> thresh_window_;
   std::shared_ptr<gui_info_t> file_info_window_;

   std::shared_ptr<image_t> image_;
};