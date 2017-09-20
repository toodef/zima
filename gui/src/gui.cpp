#include "../include/gui.hpp"
#include "qt_backend/qt_backend.hpp"

gui_t::gui_t()
{
   backend_ = std::make_unique<qt_backend_t>();
}

gui_t & gui_t::instance()
{
   static gui_t gui;
   return gui;
}

app_ptr_t gui_t::create_app(int argc, char ** argv, std::string const & name)
{
   return backend_->create_app(argc, argv, name);
}
