#ifndef COLORITEMDELEGATE_H
#define COLORITEMDELEGATE_H

#include <QStyledItemDelegate>

class ColorItemDelegate : public QStyledItemDelegate
{
  Q_OBJECT
public:
  ColorItemDelegate(QObject *parent = nullptr);
  ~ColorItemDelegate();

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

signals:

public slots:
};

#endif // COLORITEMDELEGATE_H
