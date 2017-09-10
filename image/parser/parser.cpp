#include <iostream>

#include <ImfInputFile.h>
#include <ImfChannelList.h>
#include <ImfFrameBuffer.h>
#include <ImfPixelType.h>
#include <ImfChannelListAttribute.h>
#include <half.h>
#include <ImfArray.h>
#include <cstddef>
#include <cstdio>

#include "parser.hpp"

tif_parser_t::tif_parser_t( std::string const & file )
{
   GDALAllRegister();
   data_ = GDALOpen(file.c_str(), GA_ReadOnly);

   if (data_ == NULL)
      std::cerr << "Can't read file!" << std::endl;
}

float * tif_parser_t::parse()
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

size_t tif_parser_t::width() const
{
   return x_size_;
}

size_t tif_parser_t::height() const
{
   return y_size_;
}

float tif_parser_t::min() const
{
   return min_;
}

float tif_parser_t::max() const
{
   return max_;
}

image_info_t tif_parser_t::image_info() const
{
   image_info_t result;

   result.push_back(std::make_pair("Resolution", ((std::stringstream &)(std::stringstream() << width() << "x" << height())).str()));
   result.push_back(std::make_pair("Min/max",    ((std::stringstream &)(std::stringstream() << min() << "/" << max())).str()));
   result.push_back(std::make_pair("Projection", projection()));

   return result;
}

std::string tif_parser_t::projection() const
{
   return GDALGetProjectionRef(data_);
}

exr_parser_t::exr_parser_t(std::string const &file):
     file_(file.c_str())
   , data_window_(file_.header().dataWindow())
{}

float * exr_parser_t::parse()
{
   Imf::Array2D<float> pixels;

   size_t img_width = width(), img_height = height();

   pixels.resizeErase(img_height, img_width);

   Imf::FrameBuffer frameBuffer;
   frameBuffer.insert ("Z", // name
                       Imf::Slice (Imf::FLOAT, // type
                                   (char *) (&pixels[0][0] - // base
                                             data_window_.min.x -
                                             data_window_.min.y * img_width),
                                   sizeof (pixels[0][0]) * 1, // xStride
                                   sizeof (pixels[0][0]) * img_width,// yStride
                                   1, 1, // x/y sampling
                                   0)); // fillValue
   file_.setFrameBuffer (frameBuffer);
   file_.readPixels(data_window_.min.y, data_window_.max.y);

   float * result = new float[img_width * img_height];

   size_t idx = 0;
   min_ = max_ = pixels[0][0];
   float cur_val;
   for (size_t j = 0; j < img_height; ++j)
      for (size_t i = 0; i < img_width; ++i){
         cur_val = pixels[j][i];
         result[idx++] = cur_val;

         if (cur_val < min_)
            min_ = cur_val;

         if (cur_val > max_)
            max_ = cur_val;
      }

   return result;
}

size_t exr_parser_t::width() const
{
   return data_window_.max.x - data_window_.min.x + 1;
}

size_t exr_parser_t::height() const
{
   return data_window_.max.y - data_window_.min.y + 1;
}

float exr_parser_t::min() const
{
   return min_;
}

float exr_parser_t::max() const
{
   return max_;
}

image_info_t exr_parser_t::image_info() const
{
   image_info_t result;

   result.push_back(std::make_pair("Resolution", ((std::stringstream &)(std::stringstream() << width() << "x" << height())).str()));
   result.push_back(std::make_pair("Min/max", ((std::stringstream &)(std::stringstream() << min() << "/" << max())).str()));

   return result;
}
