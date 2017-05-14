#pragma once

#include <QtWidgets/QMainWindow>
#include <QGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShader>
#include <QOpenGLTexture>
#include <QOpenGLFramebufferObject>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <iostream>

#include <../image/image.hpp>

class renderer_t : public QGLWidget
{
   Q_OBJECT

public:
   explicit renderer_t( QWidget * parent = 0 );
   ~renderer_t();

   void set_image( std::shared_ptr<image_t> const & image );

   void set_min_threshold(float min);
   void set_max_threshold(float max);

protected:
   void initializeGL() override;
   void paintGL() override;
   void resizeGL(int width, int height) override;

private:
   std::shared_ptr<QOpenGLVertexArrayObject> vertex_array_obj_;
   std::shared_ptr<QOpenGLBuffer> vertex_buffer_, index_buffer_;

   std::shared_ptr<QOpenGLShader> vertex_shader_, fragment_shader_;
   std::shared_ptr<QOpenGLShaderProgram> program_;

   std::shared_ptr<QOpenGLTexture> texture_;

   std::shared_ptr<image_t> image_;

   float min_, max_;

   // Shader sources
   const std::string vertex_shader_src_ = "#version 330\n"
         "layout(location = 0) in vec2 position; "
         "layout(location = 2) in vec2 texcoord; "
         "out vec2 Texcoord; "
         "void main() "
         "{ "
         "   Texcoord = texcoord; "
         "   gl_Position = vec4(position, 0.0, 1.0); "
         "}";

   const std::string fragment_shader_src_ = "#version 330\n"
      "in vec2 Texcoord;"
      "uniform sampler2D tex;"
      "uniform float min;"
      "uniform float max;"
      "out vec4 out_color;"
      "void main()"
      "{"
      "   float val = texture2D(tex, Texcoord).r;"
      "   if (val < min || val > max)"
      "      out_color = vec4(0, 0, 0, 1);"
      "   else{"
      "      val = (val - min) / (max - min);"
      "      out_color = vec4(val, val, val, 1);"
      "   }"
      "}";
};