#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSlider>
#include <QtWidgets/QHeaderView>
#include "gui_unit.hpp"

gui_threshold_t::gui_threshold_t( std::shared_ptr<QMenu> & parent, std::shared_ptr<image_t> image, std::shared_ptr<renderer_t> renderer ):
     image_(image)
   , renderer_(renderer)
   , min_slider_val_(0)
   , max_slider_val_(1)
   , resize_mode_(false)
{
   threshold_.reset(new QAction("Threshold"));
   threshold_->setShortcut(tr("Ctrl+T"));
   connect(threshold_.get(), SIGNAL(triggered()), this, SLOT(show()));
   parent->addAction(threshold_.get());

   setMinimumSize(10, 10);
   setMinimumSize(120, 200);
   setWindowTitle("Threshold");

   QHBoxLayout *layout = new QHBoxLayout();

   min_ = image_->get_min();
   max_ = image_->get_max();

   min_slider_.reset(new QSlider());
   min_slider_->setMinimum(0);
   min_slider_->setSingleStep(1);
   connect(min_slider_.get(), SIGNAL(valueChanged(int)), this, SLOT(set_min(int)));
   layout->addWidget(min_slider_.get());

   max_slider_.reset(new QSlider());
   max_slider_->setMinimum(0);
   max_slider_->setSingleStep(1);
   connect(max_slider_.get(), SIGNAL(valueChanged(int)), this, SLOT(set_max(int)));
   layout->addWidget(max_slider_.get());
   setLayout(layout);
}

gui_threshold_t::~gui_threshold_t()
{
}

void gui_threshold_t::set_min(int val)
{
   if (resize_mode_)
      return;

   min_slider_val_ = (float)val / slider_length_;

   if (min_slider_val_ > max_slider_val_) {
      min_slider_val_ = max_slider_val_;
      min_slider_->setValue((int) (min_slider_val_ * slider_length_));
   }

   renderer_->set_min_threshold(min_ + (max_ - min_) * min_slider_val_);
   renderer_->update();
}

void gui_threshold_t::set_max(int val)
{
   if (resize_mode_)
      return;

   max_slider_val_ = (float)val / slider_length_;

   if (max_slider_val_ < min_slider_val_) {
      max_slider_val_ = min_slider_val_;
      max_slider_->setValue((int) (max_slider_val_ * slider_length_));
   }

   renderer_->set_max_threshold(min_ + (max_ - min_) * max_slider_val_);
   renderer_->update();
}

void gui_threshold_t::resizeEvent(QResizeEvent * event)
{
   QSize size = min_slider_->size();

   slider_length_ = (size_t) size.height();

   min_slider_->setMaximum(slider_length_);
   max_slider_->setMaximum(slider_length_);
   min_slider_->setMinimum(0);
   max_slider_->setMinimum(0);

   resize_mode_ = true;
   min_slider_->setValue((int) (min_slider_val_ * slider_length_));
   max_slider_->setValue((int) (max_slider_val_ * slider_length_));
   resize_mode_ = false;
}

gui_info_t::gui_info_t(std::shared_ptr<QMenu> &parent, std::shared_ptr<image_t> image) : image_(image)
{
   file_info_.reset(new QAction("Info"));
   file_info_->setShortcut(tr("Ctrl+I"));
   connect(file_info_.get(), SIGNAL(triggered()), this, SLOT(file_info()));
   parent->addAction(file_info_.get());
}

void gui_info_t::file_info()
{
   resize(300, 200);
   setWindowTitle("File info");

   verticalHeader()->setVisible(false);
   horizontalHeader()->setVisible(false);

   horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

   setFocusPolicy(Qt::NoFocus);

   setRowCount(2);
   setColumnCount(2);

   if (image_) {
      image_info_t info = image_->get_info();
      setItem(0, 0, new QTableWidgetItem("Resolution"));
      setItem(0, 1, new QTableWidgetItem((std::to_string(info.width) + "x" + std::to_string(info.height)).c_str()));
      setItem(1, 0, new QTableWidgetItem("Min/Max values"));
      setItem(1, 1, new QTableWidgetItem((std::to_string(info.min_val) + "/" + std::to_string(info.max_val)).c_str()));
   }

   show();
}

