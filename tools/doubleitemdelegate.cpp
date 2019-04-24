#include "tools/doubleitemdelegate.h"
#include <QLineEdit>

DoubleItemDelegate::DoubleItemDelegate(QObject *parent) :
  QStyledItemDelegate(parent),
  _validator()
{

}

DoubleItemDelegate::~DoubleItemDelegate()
{

}

QWidget *DoubleItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  (void) option;
  QLineEdit *line = new QLineEdit(parent);
  QString text = index.data(Qt::EditRole).toString();
  line->setText(text);
  line->setValidator(&_validator);
  return line;
}

void DoubleItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QLineEdit *line = qobject_cast<QLineEdit *>(editor);
  Q_ASSERT(line);
  // get the index of the text in the combobox that matches the current value of the item
  const QString currentText = index.data(Qt::EditRole).toString();
  line->setText(currentText);
}

void DoubleItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  QLineEdit *line = qobject_cast<QLineEdit *>(editor);
  Q_ASSERT(line);
  model->setData(index, line->text(), Qt::EditRole);
}
