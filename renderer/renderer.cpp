#include <image/parser/parser.hpp>
#include "renderer.hpp"

renderer_t::renderer_t(QWidget * parent)
{
}

void renderer_t::initializeGL()
{
   vertex_array_obj_ = new QOpenGLVertexArrayObject();
   vertex_array_obj_->create();
   vertex_array_obj_->bind();

   vertex_buffer_ = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
   vertex_buffer_->create();
   vertex_buffer_->setUsagePattern(QOpenGLBuffer::StaticDraw);
   vertex_buffer_->bind();

   GLfloat vertices[] = {
         //Position   Texcoords
         -1.f, -1.f,  0.f, 1.f, // bottom-left  - 0
          1.f, -1.f,  1.f, 1.f, // bottom-right - 1
          1.f,  1.f,  1.f, 0.f, // top-right    - 2
         -1.f,  1.f,  0.f, 0.f, // top-left     - 3
   };

   vertex_buffer_->allocate(vertices, sizeof(vertices));

   index_buffer_ = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
   index_buffer_->create();
   index_buffer_->setUsagePattern(QOpenGLBuffer::StaticDraw);
   index_buffer_->bind();

   GLuint elements[] = {
         0, 1, 2, 3
   };

   index_buffer_->allocate(elements, sizeof(elements));

   index_buffer_->bind();
   vertex_buffer_->bind();
   vertex_array_obj_->bind();

   vertex_shader_ = new QOpenGLShader(QOpenGLShader::Vertex);
   vertex_shader_->compileSourceCode(vertex_shader_src_.c_str());
   fragment_shader_ = new QOpenGLShader(QOpenGLShader::Fragment);
   fragment_shader_->compileSourceCode(fragment_shader_src_.c_str());

   program_.addShader(vertex_shader_);
   program_.addShader(fragment_shader_);
   program_.link();
   program_.bind();

   program_.enableAttributeArray(0);
   program_.setAttributeBuffer(0, GL_FLOAT, 0, 2, sizeof(float) * 4);
   program_.enableAttributeArray(2);
   program_.setAttributeBuffer(2, GL_FLOAT, sizeof(float) * 2, 2, sizeof(float) * 4);

//   parser_t parser("sample.tif");
//   float ** image = parser.parse();

   texture_ = new QOpenGLTexture(QImage("Lenna.png"));
   texture_->bind();
}

void renderer_t::paintGL()
{
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);

   index_buffer_->bind();
   vertex_buffer_->bind();
   vertex_array_obj_->bind();
   program_.bind();
   texture_->bind();

   glDrawArrays(GL_QUADS, 0, 4);
}

void renderer_t::resizeGL(int width, int height)
{
   QGLWidget::resizeGL(width, height);
}

renderer_t::~renderer_t()
{
   vertex_buffer_->release();
   vertex_buffer_->destroy();
}