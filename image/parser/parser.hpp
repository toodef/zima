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

private:
   geometry_t geometry_;

   GDALDatasetH data_;
};
