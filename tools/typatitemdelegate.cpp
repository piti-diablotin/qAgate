#include "typatitemdelegate.h"
#include <QComboBox>
#include "base/mendeleev.hpp"

TypatItemDelegate::TypatItemDelegate(QObject *parent) :
  QStyledItemDelegate(parent),
  _znucl()
{

}

TypatItemDelegate::~TypatItemDelegate()
{

}

QWidget *TypatItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  (void) option;
  (void) index;
  QComboBox *cb = new QComboBox(parent);
  for (int t = 0; t < _znucl.size(); ++t)
    cb->addItem(Agate::mendeleev::name[_znucl[t]],t+1);
  return cb;
}

void TypatItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QComboBox *cb = qobject_cast<QComboBox *>(editor);
  Q_ASSERT(cb);
  // get the index of the text in the combobox that matches the current value of the item
  const QString currentText = index.data(Qt::EditRole).toString();
  const int cbIndex = cb->findText(currentText);
  // if it is valid, adjust the combobox
  if (cbIndex >= 0)
    cb->setCurrentIndex(cbIndex);
}

void TypatItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  QComboBox *cb = qobject_cast<QComboBox *>(editor);
  Q_ASSERT(cb);
  model->setData(index, cb->currentText(), Qt::EditRole);
}

void TypatItemDelegate::setTypat(const std::vector<int> znucl)
{
  _znucl = QVector<int>::fromStdVector(znucl);
}
