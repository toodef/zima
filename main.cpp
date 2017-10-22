#include <gui.hpp>
#include <renderer/renderer.hpp>
#include <image/image.hpp>

class color_range_t{
public:
   explicit color_range_t(main_window_ptr_t & window, gl_layout_ptr_t const & gl_layout) : gl_layout_(gl_layout){
      dock_ = window->add_dock_window("Color range", WS_right);

      dock_->start_horisontal();
      min_trackbar_ = dock_->add_track_bar();
      max_trackbar_ = dock_->add_track_bar();
      dock_->cancel();

      min_line_edit_ = dock_->add_line_edit("min", EP_horisontal);
      max_line_edit_ = dock_->add_line_edit("max", EP_horisontal);

      min_line_edit_->set_content_type(LE_double);
      max_line_edit_->set_content_type(LE_double);

      min_trackbar_->bind_object(min_line_edit_);
      max_trackbar_->bind_object(max_line_edit_);
      min_line_edit_->bind_object(min_trackbar_);
      max_line_edit_->bind_object(max_trackbar_);
   }

   void show(){
      dock_->show();
   }

   void add_renderer(renderer_ptr_t const & renderer){
      renderer_ = renderer;

      min_trackbar_->add_callback([this](float min){this->renderer_->set_min_threshold(min); this->gl_layout_->redraw();});
      max_trackbar_->add_callback([this](float max){this->renderer_->set_max_threshold(max); this->gl_layout_->redraw();});

      min_trackbar_->set_max(renderer_->get_max_threshold());
      max_trackbar_->set_max(renderer_->get_max_threshold());
      min_trackbar_->set_value(renderer_->get_min_threshold());
      max_trackbar_->set_value(renderer_->get_max_threshold());
   }

private:
   dock_window_ptr_t dock_;

   line_edit_ptr_t min_line_edit_, max_line_edit_;
   track_bar_ptr_t min_trackbar_, max_trackbar_;

   renderer_ptr_t renderer_;
   gl_layout_ptr_t gl_layout_;
};

typedef std::shared_ptr<color_range_t> color_range_ptr_t;

class zima_t{
public:
   zima_t(int argc, char ** argv): gui_(gui_t::instance()){
      renderer_ = std::make_shared<renderer_t>();

      app_ = gui_.create_app(argc, argv, "ZIma");
      gl_layout_ = app_->window()->add_gl_layout();

      file_menu_ = app_->window()->add_menu_item("File");
      open_file_action_ = file_menu_->add_menu_action("Open file");
      open_file_action_->set_callback(std::bind(&zima_t::open_file, this));

      view_menu_ = app_->window()->add_menu_item("View");
      color_range_action_ = view_menu_->add_menu_action("Color range");
      color_range_action_->set_callback(std::bind(&zima_t::show_color_range, this));

      about_menu_ = app_->window()->add_menu_item("About");

      using namespace std::placeholders;

      gl_layout_->set_init_callback(std::bind(&zima_t::init_renderer, this));
      gl_layout_->set_resize_callback(std::bind(&zima_t::resize_renderer, this, _1, _2));
      gl_layout_->set_redraw_callback(std::bind(&zima_t::redraw, this));
      gl_layout_->set_on_mouse_move_callback(std::bind(&zima_t::mouse_move, this, _1, _2));
      gl_layout_->set_on_wheel_callback(std::bind(&zima_t::wheel, this, _1, _2, _3));
   }

   int start(){
      return app_->start();
   }

private:
   void init_renderer(){
      renderer_->initialize();
   }

   void resize_renderer(int w, int h){
      renderer_->resize(w, h);
   }

   void redraw(){
      renderer_->redraw();
   }

   void mouse_move(int x, int y){
      renderer_->on_mouse_move(x, y);
   }

   void wheel(int delta, int x, int y){
      renderer_->on_wheel(delta, x, y);
   }

   void show_color_range(){
      if (!color_range_) {
         color_range_ = std::make_shared<color_range_t>(app_->window(), gl_layout_);
         color_range_->add_renderer(renderer_);
      }

      color_range_->show();
   }

   void open_file(){
      if (!file_dialog_)
         file_dialog_ = app_->window()->add_file_dialog("Open file");

      image_ = std::make_shared<zimage_t>(file_dialog_->get_file());
      renderer_->set_image(image_);
      renderer_->redraw();
   }

   gui_t & gui_;
   app_ptr_t app_;
   gl_layout_ptr_t gl_layout_;
   menu_ptr_t file_menu_, view_menu_, about_menu_;
   menu_action_ptr_t open_file_action_, color_range_action_;

   file_dialog_ptr_t file_dialog_;

   std::shared_ptr<renderer_t> renderer_;
   std::shared_ptr<zimage_t> image_;

   color_range_ptr_t color_range_;
};

int main( int argc, char ** argv )
{
   zima_t zima(argc, argv);
   return zima.start();
}
