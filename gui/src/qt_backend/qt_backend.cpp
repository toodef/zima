#include <QVBoxLayout>
#include <iostream>

#include "qt_backend.hpp"

window_ptr_t qt_backend_t::create_window(std::string const & title)
{
   return window_ptr_t(new qt_window_t(title));
}

menu_ptr_t qt_backend_t::create_menu(window_ptr_t &parent)
{
   return menu_ptr_t(new qt_menu_t());
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
   main_window_ = std::make_shared<qt_window_t>(name);

   Q_INIT_RESOURCE(resource);
}

void qt_app_t::start()
{
   main_window_->show();
   app_->exec();
}

window_ptr_t qt_app_t::window()
{
   return main_window_;
}

qt_window_t::qt_window_t(std::string const & title):
     title_(title)
   , last_layout_idx_(0)
{
   window_.setWindowTitle(title.c_str());

   QIcon ico(":/bin/logo_128.png");
   window_.setWindowIcon(ico);

   auto layout = new QVBoxLayout();
   layouts_.push_back(layout);
   auto widget = new QWidget();
   widget->setLayout(layout);
   window_.setCentralWidget(widget);


}

void qt_window_t::show()
{
   window_.show();
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
   for (auto & item: layouts_)
      delete item;
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

void qt_window_t::resize(size_t width, size_t height)
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