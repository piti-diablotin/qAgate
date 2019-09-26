#include "coloritemdelegate.h"
#include <QColorDialog>

ColorItemDelegate::ColorItemDelegate(QObject *parent) :
  QStyledItemDelegate(parent)
{

}

ColorItemDelegate::~ColorItemDelegate()
{

}

QWidget *ColorItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  (void) option;
  (void) index;
  QColorDialog  *colorDialog = new QColorDialog(parent);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog);
  colorDialog->setWindowTitle(tr("Pick a color"));
  return colorDialog;
}

void ColorItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QColorDialog *colorDialog = qobject_cast<QColorDialog*>(editor);
  Q_ASSERT(colorDialog);
  QColor current(index.data(Qt::DisplayRole).toString());
  colorDialog->setCurrentColor(current);
}

void ColorItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  QColorDialog *colorDialog = qobject_cast<QColorDialog*>(editor);
  Q_ASSERT(colorDialog);
  QColor color = colorDialog->currentColor();
  model->setData(index,color.name(QColor::HexRgb),Qt::DisplayRole);
  model->setData(index,color.name(QColor::HexRgb),Qt::UserRole);
  model->setData(index,color,Qt::BackgroundRole);
}
