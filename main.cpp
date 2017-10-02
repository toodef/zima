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

int main( int argc, char ** argv )
{
   image = std::make_shared<zimage_t>("D:\\projects\\tif-viewer\\working_dir_release\\d0.exr");
   renderer = std::make_shared<renderer_t>();

   gui_t & gui = gui_t::instance();

   app_ptr_t app = gui.create_app(argc, argv, "ZIma");
   auto g = app->window()->add_gl_layout();

   g->set_init_callback(init_renderer);
   g->set_resize_callback(resize_renderer);
   g->set_redraw_callback(redraw);
   g->set_on_mouse_move_callback(mouse_move);
   g->set_on_wheel_callback(wheel);

   app->start();
}
