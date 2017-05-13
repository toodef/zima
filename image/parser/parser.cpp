#include <iostream>
#include "parser.hpp"

parser_t::parser_t( std::string const & file )
{
   GDALAllRegister();
   data_ = GDALOpen(file.c_str(), GA_ReadOnly);

   if (data_ == NULL)
      std::cerr << "Can't read file!" << std::endl;
}

parser_t::~parser_t()
{

}

float parser_t::operator ()( size_t i, size_t j )
{
   return 0;
}

float ** parser_t::parse(const geometry_t &geometry)
{
   GDALRasterBandH raster_band = GDALGetRasterBand(data_, 1);

//   GDALRasterBand * raster_band = data_->GetRasterBand(1);
//
   int x_size = GDALGetRasterBandXSize(raster_band);//raster_band->GetXSize();
//
   float * image = (float *) CPLMalloc(sizeof(float) * x_size);

   CPLErr err = GDALRasterIO(raster_band, GF_Read, 0, 0, x_size, 1, image, x_size, 1, GDT_Float32, 0, 0);
//
//   raster_band->RasterIO(GF_Read, 0, 0, x_size, 1, image, x_size, 1, GDT_Float32, 0, 0);
//
   return (float **)image;
}
