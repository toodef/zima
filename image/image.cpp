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
