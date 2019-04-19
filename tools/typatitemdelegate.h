#ifndef TYPATITEMDELEGATE_H
#define TYPATITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QVector>

class TypatItemDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  TypatItemDelegate(QObject *parent = nullptr);
  ~TypatItemDelegate();

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
  void setTypat(const std::vector<int> znucl);

private:
  QVector<int> _znucl;
};

#endif // TYPATITEMDELEGATE_H
