#pragma once

#ifdef _MSC_VER
   #include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>

#include "../image/image.hpp"

class renderer_t
{
public:
   explicit renderer_t();
   ~renderer_t();

   void set_image( std::shared_ptr<zimage_t> const & image );

   void set_min_threshold(float min);
   void set_max_threshold(float max);

   void initialize();
   void redraw();
   void resize(int width, int height);

   void on_wheel(int delta, int x, int y);
   void on_mouse_move(int x_offset, int y_offset);

private:
   void set_geometry();

   template<typename T>
   struct vec_t{
      T x, y;

      vec_t() = default;
      vec_t(T _x, T _y): x(_x), y(_y){}

      vec_t operator + (vec_t const & vec){
         return vec_t(this->x + vec.x, this->y + vec.y);
      }

      vec_t operator - (vec_t const & vec){
         return vec_t(this->x - vec.x, this->y - vec.y);
      }

      vec_t operator * (T val){
         return vec_t(this->x * val, this->y * val);
      }

      void operator += (vec_t const & vec){
         this->x += vec.x;
         this->y += vec.y;
      }
   };

   typedef vec_t<int> veci_t;
   typedef vec_t<float> vecf_t;

   struct frame_t{
      frame_t();

      void move(veci_t const & offset);
      void scale(float scale, veci_t const & position);
      void resize(size_t width, size_t height);

      void set_image_size(size_t img_width, size_t img_height);

      float get_left(){return left_;}
      float get_right(){return right_;}
      float get_top(){return top_;}
      float get_bottom(){return bottom_;}

      void calc_frame();

      vecf_t center_;
      float scale_;

      float min_frame_x_, min_frame_y_;
      size_t img_width_, img_height_;

      float left_, right_, bottom_, top_;

      size_t width_, height_;
   };

   std::shared_ptr<zimage_t> image_;

   float min_, max_;

   std::shared_ptr<frame_t> frame_;

   float min_frame_x_, min_frame_y_;

   bool is_init_;

   size_t width_, height_;

   GLuint shader_program_;
   GLint min_uniform_, max_uniform_;
   GLuint texture_;
   GLuint vertex_buffer_;

   // Shader sources
   const std::string vertex_shader_src_ = "#version 330\n"
         "layout(location = 0) in vec2 position;"
         "layout(location = 2) in vec2 texcoord;"
         "out vec2 out_texcoord; "
         "void main()"
         "{"
         "   out_texcoord = texcoord;"
         "   gl_Position = vec4(position, 0.0, 1.0);"
         "}";

   const std::string fragment_shader_src_ = "#version 330\n"
      "in vec2 out_texcoord;"
      "uniform sampler2D tex;"
      "uniform float min;"
      "uniform float max;"
      "out vec4 out_color;"
      "void main()"
      "{"
      "   float val = texture(tex, out_texcoord).r;"
      "   if (val < min || val > max)"
      "      out_color = vec4(0, 0, 0, 1);"
      "   else{"
      "      val = (val - min) / (max - min);"
      "      out_color = vec4(val, 1 - val, 0, 1);"
      "   }"
      "}";
};