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

   file_info_.reset(new QAction("Info"));
   file_info_->setShortcut(tr("Ctrl+I"));
   connect(file_info_.get(), SIGNAL(triggered()), this, SLOT(file_info()));
   file_menu_->addAction(file_info_.get());

   view_menu_.reset(menu_bar->addMenu("View"));
   threshold_.reset(new QAction("Threshold"));
   threshold_->setShortcut(tr("Ctrl+T"));
   connect(threshold_.get(), SIGNAL(triggered()), this, SLOT(threshold()));
   view_menu_->addAction(threshold_.get());

   about_menu_.reset(menu_bar->addMenu("About"));
}

void gui_t::open_file()
{
   file_dialog_.reset(new QFileDialog(this, "Open file", "", "GTiff files (*.tif)"));
   file_dialog_->show();
}

void gui_t::threshold()
{
   thresh_window_.reset(new QWidget());

   thresh_window_->setMinimumSize(10, 10);
   thresh_window_->resize(120, 200);
   thresh_window_->setWindowTitle("Threshold");

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
   if(thresh_window_ && thresh_window_->isVisible()) {
      thresh_window_->close();
   }

   if(file_info_window_ && file_info_window_->isVisible()) {
      file_info_window_->close();
   }
}

void gui_t::file_info()
{
   file_info_window_.reset(new QTableWidget());

   file_info_window_->resize(300, 200);
   file_info_window_->setWindowTitle("File info");

   file_info_window_->verticalHeader()->setVisible(false);
   file_info_window_->horizontalHeader()->setVisible(false);

   file_info_window_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

   file_info_window_->setFocusPolicy(Qt::NoFocus);

   file_info_window_->setRowCount(2);
   file_info_window_->setColumnCount(2);

   if (image_) {
      image_info_t info = image_->get_info();
      file_info_window_->setItem(0, 0, new QTableWidgetItem("Resolution"));
      file_info_window_->setItem(0, 1, new QTableWidgetItem((std::to_string(info.width) + "x" + std::to_string(info.height)).c_str()));
      file_info_window_->setItem(1, 0, new QTableWidgetItem("Min/Max values"));
      file_info_window_->setItem(1, 1, new QTableWidgetItem((std::to_string(info.min_val) + "/" + std::to_string(info.max_val)).c_str()));
   }

   file_info_window_->show();
}

