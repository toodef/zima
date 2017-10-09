#include <gui.hpp>
#include <renderer/renderer.hpp>
#include <image/image.hpp>

std::shared_ptr<renderer_t> renderer;
std::shared_ptr<zimage_t> image;

void init_renderer(){
   renderer->initialize();
   renderer->set_image(image);
}

void resize_renderer(int w, int h){
   renderer->resize(w, h);
}

void redraw(){
   renderer->redraw();
}

void mouse_move(int x, int y){
   renderer->on_mouse_move(x, y);
}

void wheel(int delta, int x, int y){
   renderer->on_wheel(delta, x, y);
}

class color_range_t{
public:
   color_range_t(main_window_ptr_t & window) {
      dock_ = window->add_dock_window("Color range", WS_right);

      dock_->start_horisontal();
      min_trackbar_ = dock_->add_track_bar();
      max_trackbar_ = dock_->add_track_bar();
      dock_->cancel();

      min_line_edit_ = dock_->add_line_edit("min", EP_horisontal);
      max_line_edit_ = dock_->add_line_edit("max", EP_horisontal);
   }

   dock_window_ptr_t dock_;

   line_edit_ptr_t min_line_edit_, max_line_edit_;
   track_bar_ptr_t min_trackbar_, max_trackbar_;
};

void cal(){
   std::cout << "cal";
}

int main( int argc, char ** argv )
{
   image = std::make_shared<zimage_t>("D:\\projects\\tif-viewer\\working_dir_release\\d0.exr");
   renderer = std::make_shared<renderer_t>();

   gui_t & gui = gui_t::instance();

   app_ptr_t app = gui.create_app(argc, argv, "ZIma");
   auto g = app->window()->add_gl_layout();
   auto m1 = app->window()->add_menu_item("File");
//   auto m2 = app->window()->add_menu_item("View");
//   auto m3 = app->window()->add_menu_item("About");
   auto m12 = m1->add_menu("Asasa");
   auto m121 = m12->add_menu_action("Asasa2");
   m121->set_callback(cal);

   color_range_t color_range(app->window());

   g->set_init_callback(init_renderer);
   g->set_resize_callback(resize_renderer);
   g->set_redraw_callback(redraw);
   g->set_on_mouse_move_callback(mouse_move);
   g->set_on_wheel_callback(wheel);

   app->start();
}
