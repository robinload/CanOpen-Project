#ifndef CARDDELEGATE_H
#define CARDDELEGATE_H

#include <QStyledItemDelegate>
#include <QSize>

class CardDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit CardDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
    void zeroButtonClicked(const QModelIndex &index) const;  // Accept QModelIndex
    void restoreButtonClicked(const QModelIndex &index) const;  // Accept QModelIndex

};

#endif // CARDDELEGATE_H
