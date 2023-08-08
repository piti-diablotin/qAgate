#include "adddos.h"
#include "ui_adddos.h"
#include <QColorDialog>

AddDos::AddDos(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::AddDos),
  _db(nullptr),
  _hasColor(false)
{
  ui->setupUi(this);
}

AddDos::~AddDos()
{
  delete ui;
}

void AddDos::on_atom_currentIndexChanged(int index)
{
  (void) index;
  unsigned atom = static_cast<int>(ui->atom->currentData(Qt::UserRole).toInt());
  ElectronDos dos = _db->atom(atom);
  ui->colorLabel->setEnabled(true);
  ui->color->setEnabled(true);
  ui->labelLabel->setEnabled(true);
  ui->label->setEnabled(true);
  if (atom == 0)
  {
    ui->magnetic->setDisabled(true);
    ui->magneticLabel->setDisabled(true);
    ui->magnetic->setVisible(false);
    ui->magneticLabel->setVisible(false);
    if (dos.prtdos() == 5)
    {
      ui->projection->clear();
      ui->projection->addItem("Up-Up","uu");
      ui->projection->addItem("Down-Down","dd");
      ui->projection->addItem("Up-Down","ud");
      ui->projection->addItem("Down-Up","du");
      ui->projection->addItem("X","x");
      ui->projection->addItem("Y","y");
      ui->projection->addItem("Z","z");
      ui->projectionLabel->setText(tr("Projection"));
      ui->projection->setEnabled(true);
      ui->projectionLabel->setEnabled(true);
    }
    else
    {
      ui->projection->clear();
      ui->projection->setDisabled(true);
      ui->projectionLabel->setDisabled(true);
      ui->colorLabel->setDisabled(true);
      ui->color->setDisabled(true);
      ui->labelLabel->setDisabled(true);
      ui->label->setDisabled(true);
    }
  }
  else
  {
    if (dos.isProjected())
    {
      ui->projection->setEnabled(true);
      ui->projectionLabel->setEnabled(true);
      ui->projectionLabel->setText(tr("Angular moment"));
      ui->projection->clear();
      ui->projection->addItem("s","s");
      ui->projection->addItem("p","p");
      ui->projection->addItem("d","d");
      ui->projection->addItem("f","f");
      ui->projection->addItem("g","g");
      if (dos.isMResolved())
      {
        //ui->magnetic->setEnabled(true);
        ui->magneticLabel->setEnabled(true);
        ui->magnetic->setVisible(true);
        ui->magneticLabel->setVisible(true);
        this->on_projection_currentTextChanged(ui->projection->currentText());
      }
      else
      {
        ui->magnetic->setDisabled(true);
        ui->magneticLabel->setDisabled(true);
        ui->magnetic->setVisible(false);
        ui->magneticLabel->setVisible(false);
      }
    }
  }
}

void AddDos::setDb(DosDB *db)
{
  if (db==nullptr) this->close();
  _db = db;
  auto list = db->list();
  std::sort(list.begin(),list.end());
  for (auto l : list)
    if (l!=0||_db->atom(l).prtdos()==5) ui->atom->addItem(l==0?tr("Total"):tr("Atom %0").arg(l),l);
  ui->atom->setCurrentIndex(0);
}

void AddDos::on_projection_currentTextChanged(const QString &arg1)
{
  if (arg1.size() == 1)  // angular projection
  {
    int l = static_cast<int>(ui->projection->currentIndex());
    ui->magnetic->clear();
    for ( int m = -l; m <= l; ++m)
    {
      ui->magnetic->addItem(QString::number(m),m);
    }
  }
}

void AddDos::on_color_clicked()
{
  QColor newColor = QColorDialog::getColor(Qt::black,this,tr("Pick a color"),QColorDialog::DontUseNativeDialog);
  if (!newColor.isValid()) return;
  QPalette pal = ui->color->palette();
  pal.setColor(QPalette::Button,newColor);
  ui->color->setPalette(pal);
  _hasColor = true;
}

bool AddDos::hasColor() const
{
  return _hasColor;
}

QList<QStandardItem*> AddDos::item()
{
  QStandardItem *selection = new QStandardItem;
  QStandardItem *projection = new QStandardItem;
  QStandardItem *magnetic = new QStandardItem;
  QStandardItem *color = new QStandardItem;
  QStandardItem *label = new QStandardItem;

  selection->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsSelectable);
  projection->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
  magnetic->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
  color->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable);

  selection->setData(ui->atom->currentText(),Qt::DisplayRole);
  selection->setData(ui->atom->currentData(Qt::UserRole),Qt::UserRole);
  selection->setData(Qt::Checked,Qt::CheckStateRole);
  if (ui->projection->isEnabled())
  {
    projection->setData(ui->projection->currentText(),Qt::DisplayRole);
    projection->setData(ui->projection->currentData(Qt::UserRole),Qt::UserRole);
    if (ui->magnetic->isEnabled())
    {
      magnetic->setData(ui->magnetic->currentText(),Qt::DisplayRole);
      magnetic->setData(ui->magnetic->currentData(Qt::UserRole),Qt::UserRole);
    }
  }
  if (this->hasColor())
  {
    QColor colorInput = ui->color->palette().color(QPalette::Button);
    color->setData(colorInput.name(QColor::HexRgb),Qt::DisplayRole);
    color->setData(colorInput.name(QColor::HexRgb),Qt::UserRole);
    color->setData(colorInput,Qt::BackgroundRole);
  }
  if (!ui->label->text().isEmpty())
  {
    label->setData(ui->label->text(),Qt::DisplayRole);
  }
  QList<QStandardItem*> list;
  list << selection << projection << magnetic << color
           << label;
  return list;
}

void AddDos::on_magneticLabel_clicked(bool checked)
{
   ui->magnetic->setEnabled(checked);
}
