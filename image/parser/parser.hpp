#pragma once

#include <string>

#include <gdal_priv.h>
#include <ImfInputFile.h>
#include <cstddef>

//typedef std::vector<std::pair<std::string, std::string>> image_info_t;

/**
 * Abstract parser interface
 */
class parser_t{
public:
   virtual float * parse() = 0;

   virtual size_t width() const = 0;
   virtual size_t height() const = 0;

   virtual float min() = 0;
   virtual float max() = 0;

   //virtual image_info_t image_info() const = 0;

protected:
   float min_, max_;
   size_t x_size_, y_size_;
};

class tif_parser_t: public parser_t
{
public:
   explicit tif_parser_t( std::string const & file );
   ~tif_parser_t();

   float * parse() override;

   size_t width() const;
   size_t height() const;

   float min();
   float max();

   std::string projection() const;

private:
   std::string projection_;

   GDALDatasetH data_;
};

class exr_parser_t: public parser_t{
public:
   explicit exr_parser_t( std::string const & file );
   ~exr_parser_t();

   float * parse() override;

   size_t width() const override;
   size_t height() const override;

   float min() override;
   float max() override;

private:
   Imf::InputFile file_;
   Imath::Box2i data_window_;
};