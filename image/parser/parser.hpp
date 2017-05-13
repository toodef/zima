#pragma once

#include <string>

#include <gdal_priv.h>

class geometry_t
{
   size_t height, weight;
   size_t row_cnt, col_cnt;
};

class parser_t
{
public:
   parser_t( std::string const & file );
   ~parser_t();

   float operator()( size_t i, size_t j );

   float ** parse( geometry_t const & geometry = geometry_t() );

   size_t width() const;
   size_t height() const;

   float min();
   float max();

private:
   geometry_t geometry_;

   float min_, max_;
   size_t x_size_, y_size_;

   GDALDatasetH data_;
};
