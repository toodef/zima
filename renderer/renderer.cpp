#include "renderer.hpp"

renderer_t::renderer_t(QWidget * parent) :
     min_(0)
   , max_(0)
   , is_moved_(false)
   , min_frame_x_(1)
   , min_frame_y_(1)
   , intermediate_pos_(QPoint(0, 0))
{
}

void renderer_t::initializeGL()
{
   vertex_array_obj_.reset(new QOpenGLVertexArrayObject());
   vertex_array_obj_->create();
   vertex_array_obj_->bind();

   index_buffer_.reset(new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer));
   index_buffer_->create();
   index_buffer_->setUsagePattern(QOpenGLBuffer::StaticDraw);
   index_buffer_->bind();

   vertex_buffer_.reset(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer));
   vertex_buffer_->create();
   vertex_buffer_->setUsagePattern(QOpenGLBuffer::StaticDraw);

   set_geometry();

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

void renderer_t::resizeGL( int width, int height )
{
   glViewport(0, 0, width, height);

   frame_.resize(width, height);

   set_geometry();
}

renderer_t::~renderer_t()
{
}

void renderer_t::set_min_threshold(float min) { min_ = min; }

void renderer_t::set_max_threshold(float max) { max_ = max; }

void renderer_t::set_image(std::shared_ptr<image_t> const & image)
{
   image_ = image;

   min_ = image_->get_min();
   max_ = image_->get_max();

   GLuint tex;
   glGenTextures(1, &tex);
   glBindTexture(GL_TEXTURE_2D, tex);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, image_->get_width(), image_->get_height(), 0, GL_RED, GL_FLOAT, image_->get_image());
}

void renderer_t::set_geometry()
{
   vertex_buffer_->release();
   vertex_buffer_->bind();

   float   left = frame_.get_left()
         , right = frame_.get_right()
         , top = frame_.get_top()
         , bottom = frame_.get_bottom();

   GLfloat vertices[] = {
         //Position   Texcoords
         left, bottom, 0.f, 1.f, // bottom-left  - 0
         right, bottom, 1.f, 1.f, // bottom-right - 1
         right, top, 1.f, 0.f, // top-right    - 2
         left, top, 0.f, 0.f, // top-left     - 3
   };

   vertex_buffer_->allocate(vertices, sizeof(vertices));
}

void renderer_t::wheelEvent( QWheelEvent * event )
{
   float scale_factor = 1.1f;

   if (event->delta() > 0)
      frame_.scale(1.f / scale_factor, event->pos());
   else
      frame_.scale(scale_factor, event->pos());

   set_geometry();
   update();
}

void renderer_t::mouseMoveEvent( QMouseEvent * event )
{
   if (!is_moved_) {
      intermediate_pos_ = event->pos();
      is_moved_ = true;
      return;
   }

   frame_.move(intermediate_pos_, event->pos());

   intermediate_pos_ = event->pos();

   set_geometry();
   update();
}

void renderer_t::mouseReleaseEvent( QMouseEvent * event )
{
   is_moved_ = false;
}

renderer_t::frame_t::frame_t():
     center_(QVector2D(0, 0))
   , scale_(1)
   , min_frame_x_(1)
   , min_frame_y_(1)
   , left_(-1.f)
   , right_(1.f)
   , bottom_(-1.f)
   , top_(1.f)
   , width_(1)
   , height_(1)
{
}

void renderer_t::frame_t::move(QPoint const & prev_pos, QPoint const & new_pos)
{
   QVector2D prev_frame_pos(2.f * (float)prev_pos.x() / width_ - 1.f, 2.f * (1.f - (float)prev_pos.y() / height_) - 1.f);
   QVector2D new_frame_pos(2.f * (float)new_pos.x() / width_ - 1.f, 2.f * (1.f - (float)new_pos.y() / height_) - 1.f);

   center_ += new_frame_pos - prev_frame_pos;

   calc_frame();
}

void renderer_t::frame_t::scale(float scale, QPoint const & position)
{
   QVector2D pos((2.f * (float)position.x() / width_ - 1.f), (2.f * (1.f - (float)position.y() / height_) - 1.f));

   QVector2D prev_pos = (pos - center_);

   scale_ *= scale;

   if (scale_ < 1)
      scale_ = 1.f;

   QVector2D new_pos = (pos - center_) * scale;

   center_ += prev_pos - new_pos;

   calc_frame();
}

void renderer_t::frame_t::resize(size_t width, size_t height)
{
   width_ = width;
   height_ = height;

   if (width > height) {
      min_frame_x_ = (float)height / width;
      min_frame_y_ = 1;
   }
   else {
      min_frame_x_ = 1;
      min_frame_y_ = (float)width / height;
   }

   calc_frame();
}

void renderer_t::frame_t::calc_frame()
{
   left_ = -min_frame_x_ * scale_ + center_.x();

   if (left_ > -min_frame_x_){
      center_.setX(center_.x() - (left_ + min_frame_x_));
      left_ = -min_frame_x_;
   }

   right_ = min_frame_x_ * scale_ + center_.x();

   if (right_ < min_frame_x_){
      center_.setX(center_.x() - (right_ - min_frame_x_));
      right_ = min_frame_x_;
      left_ = -min_frame_x_ * scale_ + center_.x();
   }

   top_ = min_frame_y_ * scale_ + center_.y();

   if (top_ < min_frame_y_){
      center_.setY(center_.y() - (top_ - min_frame_y_));
      top_ =  min_frame_y_;
   }

   bottom_ = -min_frame_y_ * scale_ + center_.y();

   if (bottom_ > -min_frame_y_){
      center_.setY(center_.y() - (bottom_ + min_frame_y_));
      bottom_ = -min_frame_y_;
      top_ = min_frame_y_ * scale_ + center_.y();
   }
}
