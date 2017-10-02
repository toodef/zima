#pragma once

#include <string>

#include <gdal_priv.h>
#include <ImfInputFile.h>
#include <cstddef>
#include <memory>

typedef std::vector<std::pair<std::string, std::string>> image_info_t;

/**
 * Abstract parser interface
 */
class parser_t{
public:
   virtual float * parse() = 0;

   virtual size_t width() const = 0;
   virtual size_t height() const = 0;

   virtual float min() const = 0;
   virtual float max() const = 0;

   virtual image_info_t image_info() const = 0;

protected:
   float min_, max_;
   size_t x_size_, y_size_;
};

typedef std::shared_ptr<parser_t> parser_ptr_t;

class tif_parser_t: public parser_t
{
public:
   explicit tif_parser_t( std::string const & file );

   float * parse() override;

   size_t width() const;
   size_t height() const;

   float min() const override;
   float max() const override;

   image_info_t image_info() const;

private:
   std::string projection() const;

private:
   std::string projection_;

   GDALDatasetH data_;
   GDALRasterBandH raster_band_;
};

class exr_parser_t: public parser_t{
public:
   explicit exr_parser_t( std::string const & file );

   float * parse() override;

   size_t width() const override;
   size_t height() const override;

   float min() const override;
   float max() const override;

   image_info_t image_info() const;

private:
   Imf::InputFile file_;
   Imath::Box2i data_window_;
};