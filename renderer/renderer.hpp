#pragma once

#include <QtWidgets/QMainWindow>
#include <QGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShader>
#include <QOpenGLTexture>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <iostream>

class renderer_t : public QGLWidget
{
   Q_OBJECT

public:
   explicit renderer_t( QWidget * parent = 0 );
   ~renderer_t();

protected:
   void initializeGL() override;
   void paintGL() override;
   void resizeGL(int width, int height) override;

private:
   QOpenGLVertexArrayObject * vertex_array_obj_;
   QOpenGLBuffer * vertex_buffer_, * index_buffer_;

   QOpenGLShader * vertex_shader_, * fragment_shader_;
   QOpenGLShaderProgram program_;

   QOpenGLTexture * texture_;

   // Shader sources
   const std::string vertex_shader_src_ = "#version 330 core \n"
      "layout(location = 0) in vec2 position; "
//      "in vec3 color; "
      "layout(location = 2) in vec2 texcoord; "
//      "out vec3 Color; "
      "out vec2 Texcoord; "
      "void main() "
      "{ "
//      "   Color = color; "
      "   Texcoord = texcoord; "
      "   gl_Position = vec4(position, 0.0, 1.0); "
      "}";

   const std::string fragment_shader_src_ = "#version 330 core\n"
//      "in vec3 Color;"
      "in vec2 Texcoord;"
      "out vec4 outColor;"
      "uniform sampler2D tex;"
      "void main()"
      "{"
      "   vec4 cur_color = texture(tex, Texcoord);"
      "   "
      "   outColor = cur_color;"
      "}";
};