#pragma once

#include <memory>

#include "../src/gui_backend.hpp"

class gui_t{
public:
   static gui_t & instance();

   app_ptr_t create_app(int argc, char ** argv, std::string const & name);

private:
   std::unique_ptr<gui_backend_t> backend_;

   app_ptr_t app_;

private:
   gui_t();

   gui_t(gui_t const &) = delete;
   gui_t & operator= (gui_t const &) = delete;
};