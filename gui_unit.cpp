#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSlider>
#include <QtWidgets/QHeaderView>
#include "gui_unit.hpp"

gui_threshold_t::gui_threshold_t( std::shared_ptr<QMenu> & parent, std::shared_ptr<image_t> image, std::shared_ptr<renderer_t> renderer ):
     image_(image)
   , renderer_(renderer)
{
   threshold_.reset(new QAction("Threshold"));
   threshold_->setShortcut(tr("Ctrl+T"));
   connect(threshold_.get(), SIGNAL(triggered()), this, SLOT(threshold()));
   parent->addAction(threshold_.get());

   setMinimumSize(10, 10);
   resize(120, 200);
   setWindowTitle("Threshold");

   QHBoxLayout *layout = new QHBoxLayout();

   min_ = (int)image_->get_min();
   max_ = (int)image_->get_max();

   min_slider_.reset(new QSlider());
   min_slider_->setMinimum(min_);
   min_slider_->setMaximum(max_);
   connect(min_slider_.get(), SIGNAL(valueChanged(int)), this, SLOT(set_min(int)));
   layout->addWidget(min_slider_.get());

   max_slider_.reset(new QSlider());
   max_slider_->setMinimum(min_);
   max_slider_->setMaximum(max_);
   max_slider_->setValue((int)image_->get_max());
   connect(max_slider_.get(), SIGNAL(valueChanged(int)), this, SLOT(set_max(int)));
   layout->addWidget(max_slider_.get());
   setLayout(layout);
}

void gui_threshold_t::threshold()
{
   show();
}

gui_threshold_t::~gui_threshold_t()
{
}

void gui_threshold_t::set_min(int val)
{
   if (val >= max_) {
      min_ = max_ - 1;
      min_slider_->setValue(min_);
   }
   else
      min_ = val;

   renderer_->set_min_threshold(min_);
   renderer_->update();
}

void gui_threshold_t::set_max(int val)
{
   if (val <= min_) {
      max_ = min_ + 1;
      max_slider_->setValue(max_);
   }
   else
      max_ = val;

   renderer_->set_max_threshold(max_);
   renderer_->update();
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

