#include "gui_unit.hpp"

gui_threshold_t::gui_threshold_t( std::shared_ptr<QMenu> const & parent, std::shared_ptr<renderer_t> const & renderer ):
     renderer_(renderer)
   , min_(0)
   , max_(0)
   , slider_length_(1)
   , min_slider_val_(0)
   , max_slider_val_(1)
   , no_qt_call_(false)
{
   init_menu(parent);
   init_window();
}

gui_threshold_t::~gui_threshold_t() = default;

void gui_threshold_t::set_min(int val)
{
   if (no_qt_call_)
      return;

   min_slider_val_ = (float)val / slider_length_;

   if (min_slider_val_ > max_slider_val_) {
      min_slider_val_ = max_slider_val_;
      min_slider_->setValue((int) (min_slider_val_ * slider_length_));
   }

   float real_val = min_ + (max_ - min_) * min_slider_val_;

   min_line_edit_->setText(std::to_string(real_val).c_str());

   renderer_->set_min_threshold(real_val);
   renderer_->update();
}

void gui_threshold_t::set_max(int val)
{
   if (no_qt_call_)
      return;

   max_slider_val_ = (float)val / slider_length_;

   if (max_slider_val_ < min_slider_val_) {
      max_slider_val_ = min_slider_val_;
      max_slider_->setValue((int) (max_slider_val_ * slider_length_));
   }

   float real_val = min_ + (max_ - min_) * max_slider_val_;

   no_qt_call_ = true;
   max_line_edit_->setText(std::to_string(real_val).c_str());
   no_qt_call_ = false;

   renderer_->set_max_threshold(real_val);
   renderer_->update();
}

void gui_threshold_t::set_min(QString const & val)
{
   if (no_qt_call_)
      return;

   float real_val = val.toFloat();

   if (real_val < min_ || real_val > max_)
      return;

   float max_val = min_ + (max_ - min_) * max_slider_val_;
   if (real_val > max_val){
      real_val = max_val;
      min_line_edit_->setText(std::to_string(real_val).c_str());
   }

   no_qt_call_ = true;
   int slider_val = (int) (slider_length_ * (real_val - min_) / (max_ - min_));
   min_slider_->setValue(slider_val);
   min_slider_val_ = (float)slider_val / slider_length_;
   no_qt_call_ = false;

   renderer_->set_min_threshold(real_val);
   renderer_->update();
}

void gui_threshold_t::set_max(QString const & val)
{
   if (no_qt_call_)
      return;

   float real_val = val.toFloat();

   if (real_val < min_ || real_val > max_)
      return;

   float min_val = min_ + (max_ - min_) * min_slider_val_;
   if (real_val < min_val){
      real_val = min_val;
      max_line_edit_->setText(std::to_string(real_val).c_str());
   }

   no_qt_call_ = true;
   int slider_val = (int) (slider_length_ * (real_val - min_) / (max_ - min_));
   max_slider_->setValue(slider_val);
   max_slider_val_ = (float)slider_val / slider_length_;
   no_qt_call_ = false;

   renderer_->set_max_threshold(real_val);
   renderer_->update();
}

void gui_threshold_t::resizeEvent(QResizeEvent * event)
{
   QSize size = min_slider_->size();

   slider_length_ = (size_t) size.height();

   min_slider_->setMaximum((int) slider_length_);
   max_slider_->setMaximum((int) slider_length_);
   min_slider_->setMinimum(0);
   max_slider_->setMinimum(0);

   no_qt_call_ = true;
   min_slider_->setValue((int) (min_slider_val_ * slider_length_));
   max_slider_->setValue((int) (max_slider_val_ * slider_length_));
   min_line_edit_->setText(std::to_string(min_ + (max_ - min_) * min_slider_val_).c_str());
   max_line_edit_->setText(std::to_string(min_ + (max_ - min_) * max_slider_val_).c_str());
   no_qt_call_ = false;
}

void gui_threshold_t::init_window()
{
   setMinimumSize(120, 200);
   setWindowTitle("Threshold");
   setMaximumWidth(120);

   setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);

   auto main_layout = new QVBoxLayout();

   auto sliders_layout = new QHBoxLayout();
   min_slider_.reset(new QSlider());
   connect(min_slider_.get(), SIGNAL(valueChanged(int)), this, SLOT(set_min(int)));
   sliders_layout->addWidget(min_slider_.get());
   max_slider_.reset(new QSlider());
   connect(max_slider_.get(), SIGNAL(valueChanged(int)), this, SLOT(set_max(int)));
   sliders_layout->addWidget(max_slider_.get());

   auto min_layout = new QHBoxLayout();
   auto min_label = new QLabel("Min");
   min_line_edit_.reset(new QLineEdit());
   min_layout->addWidget(min_label);
   min_layout->addWidget(min_line_edit_.get());
   connect(min_line_edit_.get(), SIGNAL(textChanged(const QString &)), this, SLOT(set_min(QString const &)));

   auto max_layout = new QHBoxLayout();
   auto max_label = new QLabel("Max");
   max_line_edit_.reset(new QLineEdit());
   max_layout->addWidget(max_label);
   max_layout->addWidget(max_line_edit_.get());
   connect(max_line_edit_.get(), SIGNAL(textChanged(const QString &)), this, SLOT(set_max(QString const &)));

   main_layout->addLayout(sliders_layout);
   main_layout->addLayout(min_layout);
   main_layout->addLayout(max_layout);
   setLayout(main_layout);
}

void gui_threshold_t::init_menu(std::shared_ptr<QMenu> const & parent)
{
   threshold_.reset(new QAction("Threshold", parent.get()));
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
   file_info_.reset(new QAction("File info", parent.get()));
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

   open_file_.reset(new QAction("Open file", parent.get()));
   open_file_->setShortcut(tr("Ctrl+O"));
   connect(open_file_.get(), SIGNAL(triggered()), this, SLOT(show()));
   parent->addAction(open_file_.get());

   connect(this, SIGNAL(filesSelected(QStringList)), this, SLOT(get_file(QStringList)));
}

void gui_open_file_t::get_file(QStringList const & files)
{
   emit file_selected(files[0]);
}
