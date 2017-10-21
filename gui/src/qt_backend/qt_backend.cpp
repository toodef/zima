#include <QVBoxLayout>
#include <QMenuBar>

#include <iostream>

#include "qt_backend.hpp"

main_window_ptr_t qt_backend_t::create_window(std::string const & title)
{
   return main_window_ptr_t(new qt_main_window_t(title));
}

app_ptr_t qt_backend_t::create_app(int argc, char ** argv, std::string const & name)
{
   if (!app_)
      app_ = std::make_shared<qt_app_t>(argc, argv, name);

   return app_;
}

qt_app_t::qt_app_t(int argc, char **argv, std::string const & name):
     app_(std::make_unique<QApplication>(argc, argv))
{
   main_window_ = std::make_shared<qt_main_window_t>(name);
   app_->setAttribute( Qt::AA_UseDesktopOpenGL );

   Q_INIT_RESOURCE(resource);
}

int qt_app_t::start()
{
   main_window_->show();
   return app_->exec();
}

main_window_ptr_t qt_app_t::window()
{
   return main_window_;
}

qt_window_t::qt_window_t():
   last_layout_idx_(0)
{
   auto layout = new QVBoxLayout();
   layouts_.push_back(layout);
}

void qt_window_t::start_horisontal()
{
   auto * layout = new QHBoxLayout();
   layouts_[last_layout_idx_++]->addLayout(layout);
   layouts_.push_back(layout);
}

void qt_window_t::start_group(std::string const & group_name)
{
   QGroupBox group(QString(group_name.c_str()));
   auto * layout = new QVBoxLayout();
   layouts_[last_layout_idx_++]->addLayout(layout);
   group.setLayout(layout);
}

void qt_window_t::cancel()
{
   if (last_layout_idx_ > 0)
      last_layout_idx_--;
}

qt_window_t::~qt_window_t()
{
}

line_edit_ptr_t qt_window_t::add_line_edit(std::string const & label, std::string const & default_value, elements_interdependence_t placement)
{
   line_edit_ptr_t unit = std::make_shared<qt_line_edit_t>(label, default_value, placement);
   layouts_[last_layout_idx_]->addLayout((QBoxLayout *)unit->instance());
   return unit;
}

line_edit_ptr_t qt_window_t::add_line_edit(std::string const & label, int default_value, elements_interdependence_t placement)
{ return add_line_edit(label, std::to_string(default_value), placement); }

line_edit_ptr_t qt_window_t::add_line_edit(std::string const & label, float default_value, elements_interdependence_t placement)
{ return add_line_edit(label, std::to_string(default_value), placement); }

line_edit_ptr_t qt_window_t::add_line_edit(std::string const & label, double default_value, elements_interdependence_t placement)
{ return add_line_edit(label, std::to_string(default_value), placement); }

gl_layout_ptr_t qt_window_t::add_gl_layout()
{
   gl_layout_ptr_t unit = std::make_shared<qt_gl_layout_t>();
   layouts_[last_layout_idx_]->addLayout((QBoxLayout *)unit->instance());
   return unit;
}

track_bar_ptr_t qt_window_t::add_track_bar()
{
   track_bar_ptr_t unit = std::make_shared<qt_track_bar_t>();
   layouts_[last_layout_idx_]->addLayout((QBoxLayout *)unit->instance());
   return unit;
}

void *qt_window_t::instance()
{
   return layouts_[last_layout_idx_];
}

dock_window_ptr_t qt_main_window_t::add_dock_window(std::string const &title, window_side_t side)
{
   dock_window_ptr_t dock = std::make_shared<qt_dock_window_t>(title, shared_from_this());

   window_.addDockWidget(Qt::LeftDockWidgetArea, (QDockWidget *)dock->instance());

   return dock;
}

void * qt_main_window_t::instance()
{
   return &window_;
}

void qt_main_window_t::show()
{
   window_.show();
}

qt_main_window_t::qt_main_window_t(std::string const & title)
{
   window_obj_ = std::make_shared<qt_window_t>();

   window_.setWindowTitle(title.c_str());

   QIcon ico(":/bin/logo_128.png");
   window_.setWindowIcon(ico);
   auto widget = new QWidget();
   widget->setLayout((QBoxLayout *)window_obj_->instance());
   window_.setCentralWidget(widget);
}

menu_ptr_t qt_main_window_t::add_menu_item(std::string const &name)
{
   return menu_ptr_t(new qt_menu_t(shared_from_this(), name));
}

file_dialog_ptr_t qt_main_window_t::add_file_dialog(std::string const & title)
{
   return std::make_shared<qt_file_dialog_t>(title);
}

void qt_main_window_t::resize(size_t width, size_t height)
{
   window_.resize(width, height);
}

void * qt_line_edit_t::instance() const
{
   return layout_.get();
}

qt_line_edit_t::qt_line_edit_t(std::string const & label, std::string const & default_value, elements_interdependence_t placement)
{
   init_layout(placement);
   set_label(label);
   set_value(default_value);
}

qt_line_edit_t::qt_line_edit_t(std::string const & label, int default_value, elements_interdependence_t placement):
   qt_line_edit_t(label, std::to_string(default_value), placement)
{}

qt_line_edit_t::qt_line_edit_t(std::string const & label, float default_value, elements_interdependence_t placement):
      qt_line_edit_t(label, std::to_string(default_value), placement)
{}

qt_line_edit_t::qt_line_edit_t(std::string const & label, double default_value, elements_interdependence_t placement):
      qt_line_edit_t(label, std::to_string(default_value), placement)
{}

void qt_line_edit_t::init_layout(elements_interdependence_t placement)
{
   switch(placement){
      case EP_horisontal:
         layout_ = std::make_shared<QHBoxLayout>();
         return;

      default:
         layout_ = std::make_shared<QVBoxLayout>();
         break;
   }
}

void qt_line_edit_t::set_label(std::string const & label)
{
   text_layout_.setText(QString(label.c_str()));
   layout_->addWidget(&text_layout_);
}

void qt_line_edit_t::set_value(std::string const & value)
{
   line_edit_.setText(value.c_str());
   layout_->addWidget(&line_edit_);
}

std::string qt_line_edit_t::get_value_s() const
{
   return line_edit_.text().toStdString();
}

float qt_line_edit_t::get_value_f() const
{
   return std::stof(get_value_s());
}

qt_gl_layout_t::qt_gl_layout_t():
     layout_(std::make_shared<QHBoxLayout>())
   , is_moved_(false)
   , intermediate_pos_(QPoint(0, 0))
{
   layout_->addWidget(this);
}

void * qt_gl_layout_t::instance() const
{
   return layout_.get();
}

void qt_gl_layout_t::initializeGL()
{
   glGetError();

   if (init_callback_)
      init_callback_();
   else
      QGLWidget::initializeGL();
}

void qt_gl_layout_t::paintGL()
{
   if (redraw_callback_)
      redraw_callback_();
   else
      QGLWidget::paintGL();
}

void qt_gl_layout_t::resizeGL(int width, int height)
{
   if (resize_callback_)
      resize_callback_(width, height);
   else
      QGLWidget::resizeGL(width, height);

   update();
}

void qt_gl_layout_t::wheelEvent(QWheelEvent * event)
{
   if (on_wheel_callback_)
      on_wheel_callback_(event->delta(), event->pos().x(), event->pos().y());

   update();
}

void qt_gl_layout_t::mouseMoveEvent(QMouseEvent *event)
{
   if (!on_mouse_move_callback_)
      return;

   if (!is_moved_) {
      intermediate_pos_ = event->pos();
      is_moved_ = true;
      return;
   }

   QPoint offset = event->pos() - intermediate_pos_;

   on_mouse_move_callback_(offset.x(), offset.y());

   update();

   intermediate_pos_ = event->pos();
}

void qt_gl_layout_t::mouseReleaseEvent(QMouseEvent *event)
{
   is_moved_ = false;
}

void qt_gl_layout_t::redraw()
{
   paintGL();
   update();
}

qt_track_bar_connector_t::qt_track_bar_connector_t():
     layout_(std::make_shared<QHBoxLayout>())
   , value_(0)
   , is_value_canged_inside_(false)
   , min_(0), max_(1)
{
   layout_->addWidget(this);
   connect(this, SIGNAL(valueChanged(int)), this, SLOT(set_value_slot(int)));
   setMinimum(0);
}

void qt_track_bar_connector_t::set_min(float value) {min_ = value;}
void qt_track_bar_connector_t::set_max(float value) {max_ = value;}

void qt_track_bar_connector_t::resizeEvent(QResizeEvent * event)
{
   if (size().height() < 1)
      return;

   is_value_canged_inside_ = true;
   setMaximum(size().height());
   is_value_canged_inside_ = false;

   set_value(value_);
}

float qt_track_bar_connector_t::get_value() const
{
   return value_;
}

void * qt_track_bar_connector_t::instance()
{
   return layout_.get();
}

float qt_track_bar_connector_t::set_value(float value)
{
   value_ = value > max_ ? max_ : (value < min_ ? min_ : value);

   is_value_canged_inside_ = true;
   setValue(value_to_slider_space(value_));
   is_value_canged_inside_ = false;

   return value_;
}

void qt_track_bar_connector_t::set_callback(std::function<void(float)> const & callback)
{
   callback_ = callback;
}

void qt_track_bar_connector_t::set_value_slot(int value)
{
   if (is_value_canged_inside_) return;

   value_ = value_from_slider_space(value);
   callback_(value_);
}

float qt_track_bar_connector_t::value_from_slider_space(int value) const
{
   return min_ + (max_ - min_) * ((float)value / (float)size().height());
}

int qt_track_bar_connector_t::value_to_slider_space(float value) const
{
   return static_cast<int>(size().height() * (value_ / (max_ - min_)));
}

qt_dock_window_t::qt_dock_window_t(const std::string & title, main_window_ptr_t & parent_window) :
   dock_window_t(title, parent_window)
{
   window_ = std::make_shared<QDockWidget>(QString(title.c_str()), (QMainWindow *)parent_window->instance());

   parent_window_ = std::make_shared<qt_window_t>();
   fake_widget_ = std::make_shared<QWidget>();
   fake_widget_->setLayout((QBoxLayout *)parent_window_->instance());
   window_->setWidget(fake_widget_.get());
}

void * qt_dock_window_t::instance()
{
   return window_.get();
}

void qt_dock_window_t::show()
{
   window_->show();
}

qt_menu_t::qt_menu_t(main_window_ptr_t & parent_window, std::string const & name) : menu_t(parent_window, name)
{
   menu_.reset(((QMainWindow *)parent_window->instance())->menuBar()->addMenu(name.c_str()));
}

qt_menu_t::qt_menu_t(menu_ptr_t & parent_menu, std::string const & name) : menu_t(parent_menu, name)
{
   menu_.reset(((QMenu *)(parent_menu->instance()))->addMenu(name.c_str()));
}

void * qt_menu_t::instance()
{
   if (menu_)
      return menu_.get();
   else
      return nullptr;
}

menu_ptr_t qt_menu_t::add_menu(std::string const & name)
{
   return std::make_shared<qt_menu_t>(shared_from_this(), name);
}

menu_action_ptr_t qt_menu_t::add_menu_action(std::string const & name)
{
   return std::make_shared<qt_menu_action_t>(shared_from_this(), name);
}

qt_menu_action_t::qt_menu_action_t(menu_ptr_t & parent_menu, std::string const & name) : menu_action_t(parent_menu, name)
{
   connector_.action_ = std::make_shared<QAction>(name.c_str(), ((QMenu *)parent_menu->instance()));
   ((QMenu *)parent_menu->instance())->addAction(connector_.action_.get());
}

void * qt_menu_action_t::instance()
{
   return connector_.action_.get();
}

void qt_menu_action_t::set_callback(std::function<void()> const &callback)
{
   connector_.set_callback(callback);
}

void qt_menu_action_connector_t::on_triggered()
{
   callback_();
}

void qt_menu_action_connector_t::set_callback(std::function<void()> const &callback)
{
   callback_ = callback;
   connect(action_.get(), SIGNAL(triggered()), this, SLOT(on_triggered()));
}

qt_file_dialog_t::qt_file_dialog_t(std::string const & title) : file_dialog_t(title)
{
   file_dialog_ = std::make_shared<QFileDialog>();
   file_dialog_->setWindowTitle(title.c_str());
}

void qt_file_dialog_t::set_file_types(std::vector<std::string> const & types)
{
   QStringList filters;

   for (auto & type: types)
   filters << type.c_str();

   file_dialog_->setNameFilters(filters);
}

std::string qt_file_dialog_t::get_file()
{
   return file_dialog_->getOpenFileName().toStdString();
}

void qt_track_bar_t::set_min(float value)
{
   connector_.set_min(value);
}

void qt_track_bar_t::set_max(float value)
{
   connector_.set_max(value);
}

float qt_track_bar_t::get_value() const
{
   return connector_.get_value();
}

float qt_track_bar_t::set_value(float value)
{
   std::cout << "main" << std::endl;

   return connector_.set_value(value);
}

void qt_track_bar_t::set_callback(std::function<void(float)> const & callback)
{
   connector_.set_callback(callback);
}

void * qt_track_bar_t::instance()
{
   return connector_.instance();
}
