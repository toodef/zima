#include "gui.hpp"

gui_t::gui_t( int argc, char ** argv, QWidget * parent ) : QMainWindow(parent)
{
   setWindowTitle("ZIma");

   Q_INIT_RESOURCE(resource);
//   std::cout << QResource::registerResource("resource.rcc");
   QIcon ico(":/images/logo_128.png");
   setWindowIcon(ico);

   resize(1000, 600);

   auto central_widget = new QWidget;
   auto central_layout = new QVBoxLayout;

   renderer_.reset(new renderer_t());
   central_layout->addWidget(renderer_.get());

   central_widget->setLayout(central_layout);
   setCentralWidget(central_widget);

   init_menu(argc, argv);
}

gui_t::~gui_t() = default;

void gui_t::init_menu( int argc, char ** argv )
{
   QMenuBar * menu_bar = menuBar();
   file_menu_.reset(menu_bar->addMenu("File"));

   open_file_window_.reset(new gui_open_file_t(file_menu_));
   connect(open_file_window_.get(), SIGNAL(file_selected(QString)), this, SLOT(get_file(QString)));

   file_info_window_.reset(new gui_info_t(file_menu_));

   view_menu_.reset(menu_bar->addMenu("View"));
   thresh_window_.reset(new gui_threshold_t(view_menu_, renderer_));

   if (argc == 2){
      get_file(QString(argv[1]));
   }

   about_menu_.reset(menu_bar->addMenu("About"));
}

void gui_t::closeEvent(QCloseEvent * event)
{
   if(thresh_window_->isVisible()) {
      thresh_window_->close();
   }

   if(file_info_window_->isVisible()) {
      file_info_window_->close();
   }

   if(open_file_window_->isVisible()){
      open_file_window_->close();
   }
}

void gui_t::get_file(QString const &file)
{
   image_.reset(new zimage_t(file.toStdString()));
   renderer_->set_image(image_);
   thresh_window_->set_image(image_);
   file_info_window_->set_image(image_);
}

