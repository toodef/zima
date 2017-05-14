#include "gui.hpp"

gui_t::gui_t( QWidget * parent ) : QMainWindow(parent)
{
   setWindowTitle("Tif Viewer");

   resize(1000, 600);

   QWidget * central_widget = new QWidget;
   QVBoxLayout * central_layout = new QVBoxLayout;

   renderer_ .reset(new renderer_t());
   central_layout->addWidget(renderer_.get());

   central_widget->setLayout(central_layout);
   setCentralWidget(central_widget);

   image_.reset(new image_t("sample.tif"));

   renderer_->set_image(image_);

   init_menu();
}

gui_t::~gui_t()
{
}

void gui_t::init_menu()
{
   QMenuBar * menu_bar = menuBar();
   file_menu_.reset(menu_bar->addMenu("File"));

   open_file_.reset(new QAction("Open file"));
   open_file_->setShortcut(tr("Ctrl+O"));
   connect(open_file_.get(), SIGNAL(triggered()), this, SLOT(open_file()));
   file_menu_->addAction(open_file_.get());

   file_info_window_.reset(new gui_info_t(file_menu_, image_));

   view_menu_.reset(menu_bar->addMenu("View"));
   thresh_window_.reset(new gui_threshold_t(view_menu_, image_, renderer_));

   about_menu_.reset(menu_bar->addMenu("About"));
}

void gui_t::open_file()
{
   file_dialog_.reset(new QFileDialog(this, "Open file", "", "GTiff files (*.tif)"));
   file_dialog_->show();
}

void gui_t::closeEvent(QCloseEvent * event)
{
   if(thresh_window_->isVisible()) {
      thresh_window_->close();
   }

   if(file_info_window_->isVisible()) {
      file_info_window_->close();
   }
}


