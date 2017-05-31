#pragma once

#include <QtWidgets/QMainWindow>
#include <QtOpenGL/QGLWidget>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QAction>
#include <QtWidgets/QFileDialog>
#include <QtGui/QWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QHeaderView>

#include <cstdint>

#include "renderer/renderer.hpp"
#include "gui_unit.hpp"

class gui_t: public QMainWindow
{
   Q_OBJECT

public:
   explicit gui_t( int argc, char ** argv, QWidget * parent = 0 );
   ~gui_t() override;

private slots:
   void get_file( QString const & file );

private:
   void init_menu( int argc, char ** argv );
   void closeEvent( QCloseEvent * event ) override;

   std::shared_ptr<renderer_t> renderer_;

   std::shared_ptr<QMenu> file_menu_, view_menu_, about_menu_;

   std::shared_ptr<gui_threshold_t> thresh_window_;
   std::shared_ptr<gui_info_t> file_info_window_;
   std::shared_ptr<gui_open_file_t> open_file_window_;

   std::shared_ptr<image_t> image_;
};