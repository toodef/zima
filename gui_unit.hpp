#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QMenu>
#include <QtWidgets/QTableWidget>

#include <memory>

#include "renderer/renderer.hpp"
#include "image/image.hpp"

class gui_threshold_t : public QWidget
{
   Q_OBJECT

public:
   explicit gui_threshold_t( std::shared_ptr<QMenu> & parent, std::shared_ptr<image_t> image, std::shared_ptr<renderer_t> renderer );
   ~gui_threshold_t();

private slots:
   void threshold();

   void set_min(int val);
   void set_max(int val);

private:
   std::shared_ptr<QSlider> min_slider_, max_slider_;

   int min_, max_;

   std::shared_ptr<QAction> threshold_;

   std::shared_ptr<image_t> image_;
   std::shared_ptr<renderer_t> renderer_;
};

class gui_info_t : public QTableWidget
{
   Q_OBJECT

public:
   gui_info_t(std::shared_ptr<QMenu> & parent, std::shared_ptr<image_t> image);

private slots:
   void file_info();

private:
   std::shared_ptr<QAction> file_info_;

   std::shared_ptr<image_t> image_;
};