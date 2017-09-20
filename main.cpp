#include <gui.hpp>

int main( int argc, char ** argv )
{
   gui_t & gui = gui_t::instance();

   app_ptr_t app = gui.create_app(argc, argv, "ZIma");
   auto l = app->window()->add_line_edit("asasas", std::string("10"), EP_horisontal);

   app->start();
}
