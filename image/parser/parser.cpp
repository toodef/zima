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

float * parser_t::parse()
{
   GDALRasterBandH raster_band = GDALGetRasterBand(data_, 1);

   x_size_ = (size_t)GDALGetRasterBandXSize(raster_band);
   y_size_ = (size_t)GDALGetRasterBandYSize(raster_band);

   float * image = new float[x_size_ * y_size_];

   CPLErr err = GDALRasterIO(raster_band, GF_Read, 0, 0, (int) x_size_, (int) y_size_, image, (int) x_size_, (int) y_size_, GDT_Float32, 0, 0);

   int got_min, got_max;
   min_ = (float)GDALGetRasterMinimum(raster_band, &got_min);
   max_ = (float)GDALGetRasterMaximum(raster_band, &got_max);

   double min_max[2];

   if (!(got_max && got_min)) {
      GDALComputeRasterMinMax(raster_band, false, min_max);
      min_ = (float)min_max[0];
      max_ = (float)min_max[1];
   }

   return image;
}

size_t parser_t::width() const
{
   return x_size_;
}

size_t parser_t::height() const
{
   return y_size_;
}

float parser_t::min()
{
   return min_;
}

float parser_t::max()
{
   return max_;
}
