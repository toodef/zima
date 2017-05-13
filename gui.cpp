#include "gui.hpp"

gui_t::gui_t( QWidget * parent ) : QMainWindow(parent)
{
   setWindowTitle("Tif Viewer");

   resize(1000, 600);

   QWidget * central_widget = new QWidget;
   QVBoxLayout * central_layout = new QVBoxLayout;

   renderer_ = new renderer_t();
   central_layout->addWidget(renderer_);

   central_widget->setLayout(central_layout);
   setCentralWidget(central_widget);

   init_menu();
}

gui_t::~gui_t()
{
}

void gui_t::init_menu()
{
   QMenuBar * menu_bar = menuBar();
   file_menu_ = menu_bar->addMenu("File");
   open_file_ = new QAction("Open file");
   open_file_->setShortcut(tr("Ctrl+O"));
   connect(open_file_, SIGNAL(triggered()), this, SLOT(open_file()));
   file_menu_->addAction(open_file_);

   view_menu_ = menu_bar->addMenu("View");
   threshold_ = new QAction("Threshold");
   threshold_->setShortcut(tr("Ctrl+T"));
   connect(threshold_, SIGNAL(triggered()), this, SLOT(threshold()));
   view_menu_->addAction(threshold_);

   about_menu_ = menu_bar->addMenu("About");
}

void gui_t::open_file()
{
   file_dialog_ = new QFileDialog(this, "Open file", "", "GTiff files (*.tif)");
   file_dialog_->show();
}

void gui_t::threshold()
{
   thresh_window_ = new QWidget();

   thresh_window_->setMinimumSize(10, 10);
   thresh_window_->resize(120, 200);

   QHBoxLayout *layout = new QHBoxLayout();

   QSlider * min_slider = new QSlider();
   layout->addWidget(min_slider);
   QSlider * max_slider = new QSlider();
   layout->addWidget(max_slider);
   thresh_window_->setLayout(layout);

   thresh_window_->show();
}

void gui_t::closeEvent(QCloseEvent * event)
{
   if(thresh_window_->isVisible()) {
      thresh_window_->close();
   }
}

