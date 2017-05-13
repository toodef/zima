#pragma once

#include <string>
#include <memory>

#include "parser/parser.hpp"

struct image_info_t;

class image_t
{
public:
   image_t( std::string const & file );
   ~image_t();

   std::shared_ptr<float> & get_image();
   image_info_t get_info();

   float get_min() const;
   float get_max() const;

private:
   std::shared_ptr<float> data_;

   size_t height_, width_;

   std::shared_ptr<parser_t> parser_;
};

struct image_info_t
{
   size_t width, height;
   float min_val, max_val;
};