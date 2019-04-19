#ifndef DOUBLEITEMDELEGATE_H
#define DOUBLEITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QDoubleValidator>

class DoubleItemDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  DoubleItemDelegate(QObject *parent = nullptr);
  ~DoubleItemDelegate();

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

private:
  QDoubleValidator _validator;
};

#endif // DOUBLEITEMDELEGATE_H
