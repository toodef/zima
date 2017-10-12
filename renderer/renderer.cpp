#include "renderer.hpp"

renderer_t::renderer_t() :
     min_(0)
   , max_(0)
   , min_frame_x_(1)
   , min_frame_y_(1)
   , frame_(new frame_t())
   , is_init_(false)
   , width_(0)
   , height_(0)
{
}

void renderer_t::initialize()
{
   GLenum glew_status = glewInit();

   if (glew_status != GLEW_OK)
      std::cerr << "glewInit failed. Message: " << glewGetErrorString(glew_status) << std::endl;

   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

   glGenBuffers(1, &vertex_buffer_);

   GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
   const GLchar * v_source = vertex_shader_src_.c_str();
   glShaderSource(vertex_shader, 1, &v_source, 0);
   glCompileShader(vertex_shader);

   GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
   const GLchar * f_source = fragment_shader_src_.c_str();
   glShaderSource(fragment_shader, 1, &f_source, 0);
   glCompileShader(fragment_shader);

   shader_program_ = glCreateProgram();
   glAttachShader(shader_program_, vertex_shader);
   glAttachShader(shader_program_, fragment_shader);
   glLinkProgram(shader_program_);

   GLint program_status;
   glGetProgramiv(shader_program_, GL_LINK_STATUS, &program_status);
   if (!program_status)
      std::cerr << "Shader program doesn't linked" << std::endl;

   min_uniform_ = glGetUniformLocation(shader_program_, "min");
   max_uniform_ = glGetUniformLocation(shader_program_, "max");

   is_init_ = true;

   if (image_)
      set_image(image_);

   GLenum errCode; if((errCode=glGetError()) != GL_NO_ERROR) std::cout << "OpenGl error! - " << gluErrorString(errCode);
}

void renderer_t::redraw()
{
   glClear(GL_COLOR_BUFFER_BIT);

   if (!image_)
      return;

   glUseProgram(shader_program_);

   glBindTexture(GL_TEXTURE_2D, texture_);
   glEnable(GL_TEXTURE_2D);

   glUniform1f(min_uniform_, min_);
   glUniform1f(max_uniform_, max_);

   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(2);
   glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const GLvoid *)(sizeof(float) * 2));

   glDrawArrays(GL_QUADS, 0, 4);

   glDisableVertexAttribArray(0);
   glDisableVertexAttribArray(2);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glUseProgram(0);
}

void renderer_t::resize(int width, int height)
{
   width_ = width;
   height_ = height;

   glViewport(0, 0, width, height);

   frame_->resize((size_t)width, (size_t)height);

   set_geometry();
}

void renderer_t::set_min_threshold(float min) { min_ = min; }

void renderer_t::set_max_threshold(float max) { max_ = max; }

void renderer_t::set_image(std::shared_ptr<zimage_t> const & image)
{
   if (!image)
      return;

   image_ = image;

   min_ = image_->get_min();
   max_ = image_->get_max();

   frame_->set_image_size(image_->get_width(), image_->get_height());
   frame_->resize(width_, height_);

   glGenTextures(1, &texture_);
   glBindTexture(GL_TEXTURE_2D, texture_);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, (GLsizei)image_->get_width(), (GLsizei)image_->get_height(), 0, GL_RED, GL_FLOAT, image_->get_image());

   image_->clear_data();

   set_geometry();
}

void renderer_t::on_wheel(int delta, int x, int y)
{
   static const float scale_factor = 1.1f;

   veci_t pos(x, y);

   if (delta > 0)
      frame_->scale(1.f / scale_factor, pos);
   else
      frame_->scale(scale_factor, pos);

   set_geometry();
}

void renderer_t::on_mouse_move(int x_offset, int y_offset)
{
   frame_->move(veci_t(x_offset, y_offset));
   set_geometry();
}

void renderer_t::set_geometry()
{
   if (!is_init_)
      return;

   float   left = frame_->get_left()
         , right = frame_->get_right()
         , top = frame_->get_top()
         , bottom = frame_->get_bottom();

   GLfloat vertices[] = {
         //Position   Texcoords
         left, bottom, 0.f, 1.f, // bottom-left  - 0
         right, bottom, 1.f, 1.f, // bottom-right - 1
         right, top, 1.f, 0.f, // top-right    - 2
         left, top, 0.f, 0.f, // top-left     - 3
   };

   glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
}

renderer_t::~renderer_t()
{
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glDeleteBuffers(1, &vertex_buffer_);

   glUseProgram(0);
   glDeleteProgram(shader_program_);

   glBindTexture(GL_TEXTURE_2D, 0);
   glDeleteTextures(1, &texture_);
}

float renderer_t::get_min_threshold() const
{
   if (!image_)
      return 0;

   return image_->get_min();
}

float renderer_t::get_max_threshold() const
{
   if (!image_)
      return 0;

   return image_->get_max();
}

renderer_t::frame_t::frame_t():
     center_(vecf_t(0, 0))
   , scale_(1)
   , min_frame_x_(1)
   , min_frame_y_(1)
   , left_(-1.f)
   , right_(1.f)
   , bottom_(-1.f)
   , top_(1.f)
   , width_(1)
   , height_(1)
   , img_width_(1)
   , img_height_(1)
{
}

void renderer_t::frame_t::move(veci_t const & offset)
{
   vecf_t frame_offset(2.f * ((float)offset.x) / width_, -2.f * ((float)offset.y) / height_);

   center_ += frame_offset;

   calc_frame();
}

void renderer_t::frame_t::scale(float scale, veci_t const & position)
{
   vecf_t pos((2.f * ((float)position.x) / width_ - 1.f), (2.f * (1.f - ((float)position.y) / height_) - 1.f));

   vecf_t prev_pos = (pos - center_);

   scale_ *= scale;

   if (scale_ < 1)
      scale_ = 1.f;

   vecf_t new_pos = (pos - center_) * scale;

   center_ += prev_pos - new_pos;

   calc_frame();
}

void renderer_t::frame_t::resize(size_t width, size_t height)
{
   width_ = width;
   height_ = height;

   if (img_width_ > img_height_) {
      min_frame_x_ = 1;
      min_frame_y_ = (float)img_height_ / img_width_;
   }
   else {
      min_frame_x_ = (float)img_width_ / img_height_;
      min_frame_y_ = 1;
   }

   if (width > height) {
      min_frame_x_ *= (float)height / width;
   }
   else {
      min_frame_y_ *= (float)width / height;
   }

   float scale = (min_frame_x_ > min_frame_y_) ? min_frame_x_ : min_frame_y_;

   min_frame_x_ /= scale;
   min_frame_y_ /= scale;

   calc_frame();
}

void renderer_t::frame_t::calc_frame()
{
   left_ = -min_frame_x_ * scale_ + center_.x;

   if (left_ > -min_frame_x_){
      center_.x = center_.x - (left_ + min_frame_x_);
      left_ = -min_frame_x_;
   }

   right_ = min_frame_x_ * scale_ + center_.x;

   if (right_ < min_frame_x_){
      center_.x = center_.x - (right_ - min_frame_x_);
      right_ = min_frame_x_;
      left_ = -min_frame_x_ * scale_ + center_.x;
   }

   top_ = min_frame_y_ * scale_ + center_.y;

   if (top_ < min_frame_y_){
      center_.y = center_.y - (top_ - min_frame_y_);
      top_ =  min_frame_y_;
   }

   bottom_ = -min_frame_y_ * scale_ + center_.y;

   if (bottom_ > -min_frame_y_){
      center_.y = center_.y - (bottom_ + min_frame_y_);
      bottom_ = -min_frame_y_;
      top_ = min_frame_y_ * scale_ + center_.y;
   }
}

void renderer_t::frame_t::set_image_size(size_t img_width, size_t img_height)
{
   img_width_ = img_width;
   img_height_ = img_height;
}
