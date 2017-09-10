#pragma once

#include <string>
#include <memory>

#include "parser/parser.hpp"

struct image_info_t;

class zimage_t
{
public:
   explicit zimage_t( std::string const & file );
   ~zimage_t();

   virtual float * get_image();
   image_info_t get_info();

   virtual float get_min() const;
   virtual float get_max() const;

   virtual size_t get_width() const;
   virtual size_t get_height() const;

private:
   float * data_;

   std::shared_ptr<parser_t> parser_;
};

struct image_info_t
{
   size_t width, height;
   float min_val, max_val;
   std::string projection_;
};
