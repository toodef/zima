#include "image.hpp"

image_t::image_t( std::string const & file )
{
   parser_.reset(new parser_t(file));

   data_.reset(parser_->parse());
}

std::shared_ptr<float> & image_t::get_image()
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
