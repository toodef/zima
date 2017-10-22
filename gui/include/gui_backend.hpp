#pragma once

#include <memory>
#include <string>
#include <functional>
#include <vector>
#include <iostream>

enum elements_interdependence_t{
   EP_vertical = 0,
   EP_horisontal
};

template <typename T>
class value_object_t;
template<typename T>
using value_object_ptr_t = std::shared_ptr<value_object_t<T>>;

class value_object_float_t;
typedef std::shared_ptr<value_object_float_t> value_object_float_ptr_t;
class value_object_str_t;
typedef std::shared_ptr<value_object_str_t> value_object_str_ptr_t;

template <typename T>
class value_object_t {
public:
   virtual T get_value() const {return value_;};
   virtual T set_value(T value) {value_ = value; return value_;};

   virtual void set_min(float value) {
      for (auto & object: binded_objects_str_)
         object->set_min(value);
   }
   virtual void set_max(float value) {
      for (auto & object: binded_objects_str_)
         object->set_max(value);
   }

   virtual void add_callback(std::function<void(T)> const & callback) {callbacks_.push_back(callback);}

   virtual T set_value_by_signal(T value) {
      value_ = value;
      return value_;
   }

protected:
   std::vector<std::function<void(T)>> callbacks_;

   std::vector<value_object_str_ptr_t> binded_objects_str_;
   std::vector<value_object_float_ptr_t> binded_objects_float_;

   T value_;
};

class value_object_str_t: public value_object_t<std::string> {
public:
    virtual void bind_object(value_object_str_ptr_t const & object) {
        binded_objects_str_.push_back(object);
    }

    virtual void bind_object(value_object_float_ptr_t const & object) {
        binded_objects_float_.push_back(object);
    }
};

class value_object_float_t: public value_object_t<float> {
public:
   virtual void bind_object(value_object_float_ptr_t const & object){
       binded_objects_float_.push_back(object);
   }

   virtual void bind_object(value_object_str_ptr_t const & object){
      binded_objects_str_.push_back(object);
   }
};

enum line_edit_content_type_t{
    LE_double = 0,
    LE_int
};

class line_edit_t: public value_object_str_t {
public:
   virtual void set_label(std::string const & label) = 0;
   virtual void set_content_type(line_edit_content_type_t content_type) = 0;

   virtual void * instance() const = 0;
};

typedef std::shared_ptr<line_edit_t> line_edit_ptr_t;

class track_bar_t: public value_object_float_t {
public:
    virtual void * instance() = 0;
};

typedef std::shared_ptr<track_bar_t> track_bar_ptr_t;

class gl_layout_t{
public:
   virtual void redraw() = 0;

   virtual void * instance() const = 0;

   virtual void set_init_callback(std::function<void()> const & callback) {init_callback_ = callback;}
   virtual void set_redraw_callback(std::function<void()> const & callback) {redraw_callback_ = callback;}
   virtual void set_resize_callback(std::function<void(int, int)> const & callback) {resize_callback_ = callback;}
   virtual void set_on_wheel_callback(std::function<void(int, int, int)> const & callback) {on_wheel_callback_ = callback;}
   virtual void set_on_mouse_move_callback(std::function<void(int, int)> const & callback) {on_mouse_move_callback_ = callback;}

protected:
   std::function<void()> init_callback_, redraw_callback_;
   std::function<void(int, int, int)> on_wheel_callback_;
   std::function<void(int, int)> resize_callback_, on_mouse_move_callback_;
};

typedef std::shared_ptr<gl_layout_t> gl_layout_ptr_t;

enum window_side_t{
   WS_left = 0,
   WS_right,
   WS_top,
   WS_bottom
};

class dock_window_t;
typedef std::shared_ptr<dock_window_t> dock_window_ptr_t;

class window_t{
public:
   virtual line_edit_ptr_t add_line_edit(std::string const & label, std::string const & default_value = std::string(), elements_interdependence_t placement = EP_vertical) = 0;
   virtual line_edit_ptr_t add_line_edit(std::string const & label, int default_value, elements_interdependence_t placement = EP_vertical) = 0;
   virtual line_edit_ptr_t add_line_edit(std::string const & label, float default_value, elements_interdependence_t placement = EP_vertical) = 0;
   virtual line_edit_ptr_t add_line_edit(std::string const & label, double default_value, elements_interdependence_t placement = EP_vertical) = 0;

   virtual gl_layout_ptr_t add_gl_layout() = 0;

   virtual track_bar_ptr_t add_track_bar() = 0;

   virtual void start_horisontal() = 0;
   virtual void start_group(std::string const & group_name) = 0;
   virtual void cancel() = 0;

   virtual void * instance() = 0;
};

typedef std::shared_ptr<window_t> window_ptr_t;

class menu_t;
typedef std::shared_ptr<menu_t> menu_ptr_t;

class file_dialog_t;
typedef std::shared_ptr<file_dialog_t> file_dialog_ptr_t;

class main_window_t: public window_t {
public:
   virtual void * instance() = 0;

   virtual void resize(size_t width, size_t height) = 0;
   virtual dock_window_ptr_t add_dock_window(std::string const & title, window_side_t side) = 0;
   virtual void show() = 0;

   virtual line_edit_ptr_t add_line_edit(std::string const & label, std::string const & default_value = std::string(), elements_interdependence_t placement = EP_vertical){
      return window_obj_->add_line_edit(label, default_value, placement);
   }
   virtual line_edit_ptr_t add_line_edit(std::string const & label, int default_value, elements_interdependence_t placement = EP_vertical) {
      return window_obj_->add_line_edit(label, default_value, placement);
   }
   virtual line_edit_ptr_t add_line_edit(std::string const & label, float default_value, elements_interdependence_t placement = EP_vertical) {
      return window_obj_->add_line_edit(label, default_value, placement);
   }
   virtual line_edit_ptr_t add_line_edit(std::string const & label, double default_value, elements_interdependence_t placement = EP_vertical) {
      return window_obj_->add_line_edit(label, default_value, placement);
   }

   virtual gl_layout_ptr_t add_gl_layout() {
      return window_obj_->add_gl_layout();
   }

   virtual track_bar_ptr_t add_track_bar() {
      return window_obj_->add_track_bar();
   }

   virtual menu_ptr_t add_menu_item(std::string const & name) = 0;

   virtual file_dialog_ptr_t add_file_dialog(std::string const & title) = 0;

   virtual void start_horisontal() {
      window_obj_->start_horisontal();
   }
   virtual void start_group(std::string const & group_name) {
      window_obj_->start_group(group_name);
   }
   virtual void cancel() {
      window_obj_->cancel();
   }

protected:
   window_ptr_t window_obj_;
};

typedef std::shared_ptr<main_window_t> main_window_ptr_t;

class dock_window_t: public window_t{
public:
   explicit dock_window_t(std::string const & title, main_window_ptr_t & parent_window): parent_window_(parent_window){}

   virtual void * instance() = 0;
   virtual void show() = 0;

   virtual line_edit_ptr_t add_line_edit(std::string const & label, std::string const & default_value = std::string(), elements_interdependence_t placement = EP_vertical){
      return parent_window_->add_line_edit(label, default_value, placement);
   }
   virtual line_edit_ptr_t add_line_edit(std::string const & label, int default_value, elements_interdependence_t placement = EP_vertical) {
      return parent_window_->add_line_edit(label, default_value, placement);
   }
   virtual line_edit_ptr_t add_line_edit(std::string const & label, float default_value, elements_interdependence_t placement = EP_vertical) {
      return parent_window_->add_line_edit(label, default_value, placement);
   }
   virtual line_edit_ptr_t add_line_edit(std::string const & label, double default_value, elements_interdependence_t placement = EP_vertical) {
      return parent_window_->add_line_edit(label, default_value, placement);
   }

   virtual gl_layout_ptr_t add_gl_layout() {
      return parent_window_->add_gl_layout();
   }

   virtual track_bar_ptr_t add_track_bar() {
      return parent_window_->add_track_bar();
   }

   virtual void start_horisontal() {
      parent_window_->start_horisontal();
   }
   virtual void start_group(std::string const & group_name) {
      parent_window_->start_group(group_name);
   }
   virtual void cancel() {
      parent_window_->cancel();
   }

protected:
   window_ptr_t parent_window_;
};

class menu_action_t;
typedef std::shared_ptr<menu_action_t> menu_action_ptr_t;

class menu_t: public std::enable_shared_from_this<menu_t> {
public:
   explicit menu_t(main_window_ptr_t & parent_window, std::string const & name){}
   explicit menu_t(menu_ptr_t & parent_menu, std::string const & name){}

   virtual menu_ptr_t add_menu(std::string const & name) = 0;
   virtual menu_action_ptr_t add_menu_action(std::string const & name) = 0;

   virtual void * instance() = 0;

private:
   menu_t() = default;
};

class menu_action_t: public std::enable_shared_from_this<menu_action_t> {
public:
   explicit menu_action_t(menu_ptr_t & parent_menu, std::string const & name){}

   virtual void set_callback(std::function<void()> const & callback) = 0;

   virtual void * instance() = 0;

private:
   menu_action_t() = default;
};

class file_dialog_t{
public:
   explicit file_dialog_t(std::string const & title){};

   virtual void set_file_types(std::vector<std::string> const & types) = 0;
   virtual std::string get_file() = 0;
};

class app_t {
public:
   virtual int start() = 0;
   virtual main_window_ptr_t window() = 0;

protected:
   main_window_ptr_t main_window_;
};

typedef std::shared_ptr<app_t> app_ptr_t;

class gui_backend_t{
public:
   virtual main_window_ptr_t create_window(std::string const & title) = 0;
   virtual app_ptr_t create_app(int argc, char ** argv, std::string const & name) = 0;

protected:
   app_ptr_t app_;
};
