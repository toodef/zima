#pragma once

#include <QtWidgets/QMainWindow>
#include <QtOpenGL/QGLWidget>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLShader>
#include <QtGui/QOpenGLTexture>
#include <QtGui/QOpenGLFramebufferObject>
#include <QtGui/QWheelEvent>

#include <GL/gl.h>

#include <iostream>

#include "../image/image.hpp"

class renderer_t : public QGLWidget
{
   Q_OBJECT

public:
   explicit renderer_t( QWidget * parent = nullptr );
   ~renderer_t() override;

   void set_image( std::shared_ptr<image_t> const & image );

   void set_min_threshold(float min);
   void set_max_threshold(float max);

protected:
   void initializeGL() override;
   void paintGL() override;
   void resizeGL(int width, int height) override;

private:
   struct frame_t{
      frame_t();

      void move( QPoint const & prev_pos, QPoint const & new_pos );
      void scale( float scale, QPoint const & position );
      void resize(size_t width, size_t height);

      float get_left(){return left_;}
      float get_right(){return right_;}
      float get_top(){return top_;}
      float get_bottom(){return bottom_;}

      void calc_frame();

      QVector2D center_;
      float scale_;

      float min_frame_x_, min_frame_y_;

      float left_, right_, bottom_, top_;

      size_t width_, height_;
   };

   void set_geometry();

   void wheelEvent( QWheelEvent * event ) override;
   void mouseMoveEvent( QMouseEvent * event ) override;
   void mouseReleaseEvent( QMouseEvent * event ) override;

   std::shared_ptr<QOpenGLVertexArrayObject> vertex_array_obj_;
   std::shared_ptr<QOpenGLBuffer> vertex_buffer_, index_buffer_;

   std::shared_ptr<QOpenGLShader> vertex_shader_, fragment_shader_;
   std::shared_ptr<QOpenGLShaderProgram> program_;

   std::shared_ptr<image_t> image_;

   float min_, max_;

   QPoint intermediate_pos_;

   frame_t frame_;

   float min_frame_x_, min_frame_y_;

   bool is_moved_;

   // Shader sources
   const std::string vertex_shader_src_ = "#version 330\n"
         "layout(location = 0) in vec2 position;"
         "layout(location = 2) in vec2 texcoord;"
         "out vec2 Texcoord; "
         "void main()"
         "{"
         "   Texcoord = texcoord;"
         "   gl_Position = vec4(position, 0.0, 1.0);"
         "}";

   const std::string fragment_shader_src_ = "#version 330\n"
      "in vec2 Texcoord;"
      "uniform sampler2D tex;"
      "uniform float min;"
      "uniform float max;"
      "out vec4 out_color;"
      "void main()"
      "{"
      "   float val = texture(tex, Texcoord).r;"
      "   if (val < min || val > max)"
      "      out_color = vec4(0, 0, 0, 1);"
      "   else{"
      "      val = (val - min) / (max - min);"
      "      out_color = vec4(val, 1 - val, 0, 1);"
      "   }"
      "}";
};