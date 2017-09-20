#pragma once

#include <memory>
#include <string>

enum elements_interdependence_t{
   EP_vertical = 0,
   EP_horisontal
};

class line_edit_t{
public:
   virtual void set_label(std::string const & label) = 0;
   virtual void set_value(std::string const & value) = 0;

   virtual std::string get_value_s() const  = 0;
   virtual float get_value_f() const = 0;

   virtual void * instance() const = 0;
};

typedef std::shared_ptr<line_edit_t> line_edit_ptr_t;

class image_layout_t{
};

typedef std::shared_ptr<image_layout_t> image_layout_ptr_t;

class track_bar_t{
};

typedef std::shared_ptr<track_bar_t> track_bar_ptr_t;

class window_t;
typedef std::shared_ptr<window_t> window_ptr_t;

class window_t{
public:
   virtual void show() = 0;
   virtual void resize(size_t width, size_t height) = 0;

   virtual line_edit_ptr_t add_line_edit(std::string const & label, std::string const & default_value = std::string(), elements_interdependence_t placement = EP_vertical) = 0;
   virtual line_edit_ptr_t add_line_edit(std::string const & label, int default_value, elements_interdependence_t placement = EP_vertical) = 0;
   virtual line_edit_ptr_t add_line_edit(std::string const & label, float default_value, elements_interdependence_t placement = EP_vertical) = 0;
   virtual line_edit_ptr_t add_line_edit(std::string const & label, double default_value, elements_interdependence_t placement = EP_vertical) = 0;

   virtual void start_horisontal() = 0;
   virtual void start_group(std::string const & group_name) = 0;
   virtual void cancel() = 0;
};

class menu_t{

};

typedef std::shared_ptr<menu_t> menu_ptr_t;

class app_t{
public:
   virtual void start() = 0;
   virtual window_ptr_t window() = 0;

protected:
   window_ptr_t main_window_;
};

typedef std::shared_ptr<app_t> app_ptr_t;

class gui_backend_t{
public:
   virtual window_ptr_t create_window(std::string const & title) = 0;
   virtual menu_ptr_t create_menu(window_ptr_t & parent) = 0;
   virtual app_ptr_t create_app(int argc, char ** argv, std::string const & name) = 0;

protected:
   app_ptr_t app_;
};
