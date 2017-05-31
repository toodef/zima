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

   float * get_image();
   image_info_t get_info();

   float get_min() const;
   float get_max() const;

   size_t get_width() const;
   size_t get_height() const;

private:
   float * data_;

   std::shared_ptr<parser_t> parser_;
};

struct image_info_t
{
   size_t width, height;
   float min_val, max_val;
};