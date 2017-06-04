#pragma once

#include <string>

#include <gdal_priv.h>

class parser_t
{
public:
   explicit parser_t( std::string const & file );
   ~parser_t();

   float * parse();

   size_t width() const;
   size_t height() const;

   float min();
   float max();

private:
   float min_, max_;
   size_t x_size_, y_size_;

   GDALDatasetH data_;
};
