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
   x_size_ = (size_t)GDALGetRasterBandXSize(raster_band);//raster_band->GetXSize();
   y_size_ = (size_t)GDALGetRasterBandYSize(raster_band);//raster_band->GetXSize();
//
   float ** image = new float* [y_size_];

   for (size_t i = 0; i < y_size_; ++i) {
      image[i] = new float[x_size_];
      CPLErr err = GDALRasterIO(raster_band, GF_Read, 0, i, x_size_, 1, image[i], x_size_, 1, GDT_Float32, 0, 0);
   }
//
//   raster_band->RasterIO(GF_Read, 0, 0, x_size, 1, image, x_size, 1, GDT_Float32, 0, 0);
//

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
