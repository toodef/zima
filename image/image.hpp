#pragma once

#include <string>
#include <memory>

#include "parser/parser.hpp"

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

   parser_ptr_t parser_;
};
