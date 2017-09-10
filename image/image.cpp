#include <QFileInfo>

#include "image.hpp"

zimage_t::zimage_t( std::string const & file )
{
   auto file_info = QFileInfo(file.c_str());

   auto extension = file_info.completeSuffix();

   std::cout << extension.toStdString() << std::endl;

   parser_t * parser;

   if (extension == "exr")
      parser = new exr_parser_t(file);
   else if (extension == "tif" || extension == "tiff")
      parser = new tif_parser_t(file);

   parser_.reset(parser);

   data_ = parser_->parse();
}

float * zimage_t::get_image()
{
   return data_;
}

zimage_t::~zimage_t()
{
   delete[] data_;
}

image_info_t zimage_t::get_info()
{
   image_info_t info;

   info.width = parser_->width();
   info.height = parser_->height();
   info.min_val = parser_->min();
   info.max_val = parser_->max();
   info.projection_ = "";//parser_->projection();

   return info;
}

float zimage_t::get_min() const
{
   return parser_->min();
}

float zimage_t::get_max() const
{
   return parser_->max();
}

size_t zimage_t::get_width() const
{
   return parser_->width();
}

size_t zimage_t::get_height() const
{
   return parser_->height();
}
