#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSlider>
#include <QtWidgets/QHeaderView>
#include "gui_unit.hpp"

gui_threshold_t::gui_threshold_t( std::shared_ptr<QMenu> const & parent, std::shared_ptr<renderer_t> const & renderer ):
     renderer_(renderer)
   , min_(0)
   , max_(0)
   , slider_length_(1)
   , min_slider_val_(0)
   , max_slider_val_(1)
   , resize_mode_(false)
{
   init_menu(parent);
   init_window();
}

gui_threshold_t::~gui_threshold_t() = default;

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

void gui_threshold_t::init_window()
{
   setMinimumSize(120, 200);
   setWindowTitle("Threshold");

   QHBoxLayout *layout = new QHBoxLayout();

   min_slider_.reset(new QSlider());
   connect(min_slider_.get(), SIGNAL(valueChanged(int)), this, SLOT(set_min(int)));
   layout->addWidget(min_slider_.get());

   max_slider_.reset(new QSlider());
   connect(max_slider_.get(), SIGNAL(valueChanged(int)), this, SLOT(set_max(int)));
   layout->addWidget(max_slider_.get());
   setLayout(layout);
}

void gui_threshold_t::init_menu(std::shared_ptr<QMenu> const & parent)
{
   threshold_.reset(new QAction("Threshold"));
   threshold_->setShortcut(tr("Ctrl+T"));
   threshold_->setEnabled(false);
   connect(threshold_.get(), SIGNAL(triggered()), this, SLOT(show()));
   parent->addAction(threshold_.get());
}

void gui_threshold_t::set_image(std::shared_ptr<image_t> const & image)
{
   if (!image)
      return;

   image_ = image;
   update_sliders();

   threshold_->setEnabled(true);
}

void gui_threshold_t::update_sliders()
{
   min_ = image_->get_min();
   max_ = image_->get_max();

   min_slider_->setMinimum(0);
   min_slider_->setSingleStep(1);

   max_slider_->setMinimum(0);
   max_slider_->setSingleStep(1);
}

gui_info_t::gui_info_t(std::shared_ptr<QMenu> const & parent)
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

void gui_info_t::set_image(std::shared_ptr<image_t> const & image)
{
   image_ = image;
}

gui_open_file_t::gui_open_file_t( std::shared_ptr<QMenu> const & parent )
{
   setWindowTitle("Open file");
   setNameFilter("GTiff images (*.tif)");
   setViewMode(QFileDialog::Detail);

   open_file_.reset(new QAction("Open file"));
   open_file_->setShortcut(tr("Ctrl+O"));
   connect(open_file_.get(), SIGNAL(triggered()), this, SLOT(show()));
   parent->addAction(open_file_.get());

   connect(this, SIGNAL(filesSelected(QStringList)), this, SLOT(get_file(QStringList)));
}

void gui_open_file_t::get_file(QStringList const & files)
{
   emit file_selected(files[0]);
}
