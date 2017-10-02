#include <QFileInfo>

#include "image.hpp"

zimage_t::zimage_t( std::string const & file )
{
   auto file_info = QFileInfo(file.c_str());
   auto extension = file_info.completeSuffix();

   if (extension == "exr")
      parser_ = std::make_shared<exr_parser_t>(file);
   else if (extension == "tif" || extension == "tiff")
      parser_ = std::make_shared<tif_parser_t>(file);

   data_ = parser_->parse();
}

float * zimage_t::get_image()
{
   return data_;
}

zimage_t::~zimage_t()
{
   clear_data();
}

image_info_t zimage_t::get_info()
{
   return parser_->image_info();
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

void zimage_t::clear_data()
{
   if (data_){
      delete[] data_;
      data_ = nullptr;
   }
}
