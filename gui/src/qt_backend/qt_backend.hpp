#pragma once

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QLabel>
#include <QtOpenGL/QGLWidget>
#include <QtGui/QWheelEvent>

#include <stack>

#include "src/gui_backend.hpp"

typedef std::shared_ptr<QBoxLayout> layout_ptr_t;

class qt_line_edit_t: public line_edit_t {
public:
   explicit qt_line_edit_t(std::string const & label, std::string const & default_value = std::string(), elements_interdependence_t placement = EP_vertical);
   qt_line_edit_t(std::string const & label, int default_value, elements_interdependence_t placement = EP_vertical);
   qt_line_edit_t(std::string const & label, float default_value, elements_interdependence_t placement = EP_vertical);
   qt_line_edit_t(std::string const & label, double default_value, elements_interdependence_t placement = EP_vertical);

   void set_label(std::string const & label) override;
   void set_value(std::string const & value) override ;

   std::string get_value_s() const override ;
   float get_value_f() const override;

   void * instance() const override;

private:
   void init_layout(elements_interdependence_t placement);

private:
   std::shared_ptr<QBoxLayout> layout_;
   QLineEdit line_edit_;
   QLabel text_layout_;
};

class qt_gl_layout_t: public gl_layout_t, QGLWidget{
public:
   qt_gl_layout_t();
   void * instance() const override;

private:
   void initializeGL() override;
   void paintGL() override;
   void resizeGL(int width, int height) override;

   void wheelEvent(QWheelEvent * event) override;
   void mouseMoveEvent(QMouseEvent * event) override;
   void mouseReleaseEvent(QMouseEvent * event) override;

private:
   std::shared_ptr<QBoxLayout> layout_;

   bool is_moved_;
   QPoint intermediate_pos_;
};

class qt_window_t: public window_t{
public:
   explicit qt_window_t(std::string const & title);
   ~qt_window_t();

   void show() override;
   void resize(size_t width, size_t height) override;

   line_edit_ptr_t add_line_edit(std::string const & label, std::string const & default_value = std::string(), elements_interdependence_t placement = EP_vertical) override;
   line_edit_ptr_t add_line_edit(std::string const & label, int default_value, elements_interdependence_t placement = EP_vertical) override;
   line_edit_ptr_t add_line_edit(std::string const & label, float default_value, elements_interdependence_t placement = EP_vertical) override;
   line_edit_ptr_t add_line_edit(std::string const & label, double default_value, elements_interdependence_t placement = EP_vertical) override;

   gl_layout_ptr_t add_gl_layout() override;

   void start_horisontal() override ;
   void start_group(std::string const & group_name) override;
   void cancel() override;

private:
   QMainWindow window_;

   std::vector<QBoxLayout *> layouts_;
   size_t last_layout_idx_;

   std::string title_;
};

class qt_menu_t: public menu_t{

};

class qt_app_t: public app_t{
public:
   qt_app_t(int argc, char ** argv, std::string const & name);

   void start() override;
   window_ptr_t window() override;

private:
   std::unique_ptr<QApplication> app_;
};

class qt_backend_t: public gui_backend_t{
public:
   window_ptr_t create_window(std::string const & title) override;
   menu_ptr_t create_menu(window_ptr_t & parent) override;
   app_ptr_t create_app(int argc, char ** argv, std::string const & name) override;
};
