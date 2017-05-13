#pragma once

#include <string>
#include <memory>

#include "parser/parser.hpp"

class image_t
{
public:
   image_t( std::string const & file );
   ~image_t();

   std::shared_ptr<float> & get_image();

private:
   std::shared_ptr<float> data_;

   size_t height_, width_;

   std::shared_ptr<parser_t> parser_;
};