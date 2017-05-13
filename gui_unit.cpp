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

}

void gui_threshold_t::threshold()
{
   setMinimumSize(10, 10);
   resize(120, 200);
   setWindowTitle("Threshold");

   QHBoxLayout *layout = new QHBoxLayout();

   QSlider * min_slider = new QSlider();
   min_slider->setMinimum((int)image_->get_min());
   min_slider->setMaximum((int)image_->get_max());
   connect(min_slider, SIGNAL(valueChanged(int)), renderer_.get(), SLOT(set_min_threshold(int)));
   layout->addWidget(min_slider);

   QSlider * max_slider = new QSlider();
   max_slider->setMinimum((int)image_->get_min());
   max_slider->setMaximum((int)image_->get_max());
   max_slider->setValue((int)image_->get_max());
   layout->addWidget(max_slider);
   setLayout(layout);

   show();
}

gui_threshold_t::~gui_threshold_t()
{
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

