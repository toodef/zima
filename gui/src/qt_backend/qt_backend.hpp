#pragma once

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QLabel>
#include <QtOpenGL/QGLWidget>
#include <QtGui/QWheelEvent>
#include <QtWidgets/QSlider>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFileDialog>
#include <QAction>
#include <QMenu>

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
   layout_ptr_t layout_;
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

class qt_track_bar_connector_t: private QSlider{
   Q_OBJECT

public:
   qt_track_bar_connector_t();

   void set_min(float value);
   void set_max(float value);

   float get_value() const;
   void set_value(float value);

   void set_callback(std::function<void(float)> const & callback);

   void * instance();

private:
   void resizeEvent(QResizeEvent * event) override;

   float value_, min_, max_;
   bool is_resized_;

   layout_ptr_t layout_;

   std::function<void(float)> callback_;

private slots:
   void set_value_slot(int value);
};

class qt_track_bar_t: public track_bar_t {
public:
   qt_track_bar_t() = default;

   void set_min(float value) override;
   void set_max(float value) override;

   float get_value() const override;
   void set_value(float value) override;

   void set_callback(std::function<void(float)> const & callback) override;

   void * instance() override;

private:
   qt_track_bar_connector_t connector_;
};

class qt_window_t: public window_t{
public:
   explicit qt_window_t();
   ~qt_window_t();

   line_edit_ptr_t add_line_edit(std::string const & label, std::string const & default_value = std::string(), elements_interdependence_t placement = EP_vertical) override;
   line_edit_ptr_t add_line_edit(std::string const & label, int default_value, elements_interdependence_t placement = EP_vertical) override;
   line_edit_ptr_t add_line_edit(std::string const & label, float default_value, elements_interdependence_t placement = EP_vertical) override;
   line_edit_ptr_t add_line_edit(std::string const & label, double default_value, elements_interdependence_t placement = EP_vertical) override;

   gl_layout_ptr_t add_gl_layout() override;

   track_bar_ptr_t add_track_bar() override ;

   void start_horisontal() override ;
   void start_group(std::string const & group_name) override;
   void cancel() override;

   void * instance();

private:
   std::vector<QBoxLayout *> layouts_;
   size_t last_layout_idx_;

   std::string title_;
};

class qt_main_window_t: public main_window_t, public std::enable_shared_from_this<main_window_t>{
public:
   explicit qt_main_window_t(std::string const & title);

   void * instance() override;

   menu_ptr_t add_menu_item(std::string const & name) override;
   file_dialog_ptr_t add_file_dialog(std::string const & title) override;

   void resize(size_t width, size_t height) override;
   dock_window_ptr_t add_dock_window(std::string const & title, window_side_t side) override;
   void show() override;

private:
   QMainWindow window_;
};

class qt_dock_window_t: public dock_window_t {
public:
   qt_dock_window_t(const std::string & title, main_window_ptr_t & parent_window);

   void show() override;
   void * instance() override;

private:
   std::shared_ptr<QDockWidget> window_;
   std::shared_ptr<QWidget> fake_widget_;
};

class qt_menu_t: public menu_t {
public:
   explicit qt_menu_t(main_window_ptr_t & parent_window, std::string const & name);
   explicit qt_menu_t(menu_ptr_t & parent_menu, std::string const & name);

   menu_ptr_t add_menu(std::string const & name) override;
   menu_action_ptr_t add_menu_action(std::string const & name) override;

   void * instance() override;

private:
   std::shared_ptr<QMenu> menu_;
};

class qt_menu_action_connector_t: public QObject{
   Q_OBJECT

public:
   qt_menu_action_connector_t() = default;

   void set_callback(std::function<void()> const & callback);

   std::shared_ptr<QAction> action_;

private:
   std::function<void()> callback_;

private slots:
   Q_SLOT void on_triggered();
};

class qt_menu_action_t: public menu_action_t {
public:
   explicit qt_menu_action_t(menu_ptr_t & parent_menu, std::string const & name);

   void set_callback(std::function<void()> const & callback) override;

   void * instance() override;

private:
   std::shared_ptr<QAction> action_;

   qt_menu_action_connector_t connector_;
};

class qt_file_dialog_t: public file_dialog_t{
public:
   explicit qt_file_dialog_t(std::string const & title);

   void set_file_types(std::vector<std::string> const & types) override;
   std::string get_file();

private:
   std::shared_ptr<QFileDialog> file_dialog_;
};

class qt_app_t: public app_t{
public:
   qt_app_t(int argc, char ** argv, std::string const & name);

   int start() override;
   main_window_ptr_t window() override;

private:
   std::unique_ptr<QApplication> app_;
};

class qt_backend_t: public gui_backend_t{
public:
   main_window_ptr_t create_window(std::string const & title) override;
   app_ptr_t create_app(int argc, char ** argv, std::string const & name) override;
};
