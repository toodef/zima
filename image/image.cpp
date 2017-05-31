#include "image.hpp"

image_t::image_t( std::string const & file )
{
   parser_.reset(new parser_t(file));

   data_ = parser_->parse();
}

float * image_t::get_image()
{
   return data_;
}

image_t::~image_t()
{
}

image_info_t image_t::get_info()
{
   image_info_t info;

   info.width = parser_->width();
   info.height = parser_->height();
   info.min_val = parser_->min();
   info.max_val = parser_->max();

   return info;
}

float image_t::get_min() const
{
   return parser_->min();
}

float image_t::get_max() const
{
   return parser_->max();
}

size_t image_t::get_width() const
{
   return parser_->width();
}

size_t image_t::get_height() const
{
   return parser_->height();
}
