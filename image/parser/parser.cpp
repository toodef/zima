#include "parser.hpp"

parser_t::parser_t( std::string const & file )
{
   data_ = (GDALDataset *) GDALOpenShared(file.c_str(), GA_ReadOnly);
}

parser_t::~parser_t()
{

}

float parser_t::operator ()( size_t i, size_t j )
{
   return 0;
}
