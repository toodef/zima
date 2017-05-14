#include <image/parser/parser.hpp>
#include <QtGui/QOpenGLFramebufferObject>
#include "renderer.hpp"

renderer_t::renderer_t(QWidget * parent) :
     min_(0)
   , max_(0)
{
}

void renderer_t::initializeGL()
{
   vertex_array_obj_.reset(new QOpenGLVertexArrayObject());
   vertex_array_obj_->create();
   vertex_array_obj_->bind();

   vertex_buffer_.reset(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer));
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

   index_buffer_.reset(new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer));
   index_buffer_->create();
   index_buffer_->setUsagePattern(QOpenGLBuffer::StaticDraw);
   index_buffer_->bind();

   GLuint elements[] = {0, 1, 2, 3};

   index_buffer_->allocate(elements, sizeof(elements));

   index_buffer_->bind();
   vertex_buffer_->bind();
   vertex_array_obj_->bind();

   vertex_shader_.reset(new QOpenGLShader(QOpenGLShader::Vertex));
   vertex_shader_->compileSourceCode(vertex_shader_src_.c_str());
   fragment_shader_.reset(new QOpenGLShader(QOpenGLShader::Fragment));
   fragment_shader_->compileSourceCode(fragment_shader_src_.c_str());

   program_.reset(new QOpenGLShaderProgram());
   program_->addShader(vertex_shader_.get());
   program_->addShader(fragment_shader_.get());
   program_->link();
   program_->bind();

   program_->enableAttributeArray(0);
   program_->setAttributeBuffer(0, GL_FLOAT, 0, 2, sizeof(float) * 4);
   program_->enableAttributeArray(2);
   program_->setAttributeBuffer(2, GL_FLOAT, sizeof(float) * 2, 2, sizeof(float) * 4);

   if (image_)
      set_image(image_);
}

void renderer_t::paintGL()
{
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);

   if (image_){
      program_->setUniformValue("min", min_);
      program_->setUniformValue("max", max_);
   }

   glDrawArrays(GL_QUADS, 0, 4);
}

void renderer_t::resizeGL(int width, int height)
{
   int side = qMin(width, height);
   glViewport((width - side) / 2, (height - side) / 2, side, side);
}

renderer_t::~renderer_t()
{
}

void renderer_t::set_min_threshold(float min)
{
   min_ = min;
}

void renderer_t::set_max_threshold(float max)
{
   max_ = max;
}

void renderer_t::set_image(std::shared_ptr<image_t> const & image)
{
   image_ = image;

   min_ = image_->get_min();
   max_ = image_->get_max();

   GLuint tex;
   glGenTextures(1, &tex);
   glBindTexture(GL_TEXTURE_2D, tex);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_RED};
   glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, image_->get_width(), image_->get_height(), 0, GL_RED, GL_FLOAT, image_->get_image());
}
