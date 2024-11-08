#include "carddelegate.h"
#include <QPainter>
#include <QApplication>
#include <QStyleOptionButton>
#include <QMouseEvent>
#include <QStyle>

CardDelegate::CardDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void CardDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->save();

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, QColor(173, 216, 230));
    } else {
        painter->fillRect(option.rect, option.palette.base());
    }
    painter->setPen(Qt::black);
    painter->drawRect(option.rect);

    QStringList labels = index.data(Qt::DisplayRole).toStringList();
    QRect textRect = option.rect.adjusted(20, 5, -5, -5);
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, labels.join("\n"));

    QColor color = index.data(Qt::DecorationRole).value<QColor>();
    QRect colorRect(option.rect.left() + 5, option.rect.top() + 5, 10, 10);
    painter->fillRect(colorRect, color);

    QRect checkBoxRect = option.rect;
    checkBoxRect.setLeft(option.rect.right() - 30);
    checkBoxRect.setWidth(20);
    QStyleOptionButton checkBoxStyle;
    checkBoxStyle.rect = checkBoxRect;
    checkBoxStyle.state = index.data(Qt::CheckStateRole).toBool() ? QStyle::State_On : QStyle::State_Off;
    QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkBoxStyle, painter);

    QRect buttonRect1 = option.rect.adjusted(5, option.rect.height() - 30, -option.rect.width() / 2 - 5, -5);
    painter->setBrush(QColor(200, 200, 200));
    painter->drawRect(buttonRect1);
    painter->drawText(buttonRect1, Qt::AlignCenter, "Zero");

    QRect buttonRect2 = option.rect.adjusted(option.rect.width() / 2 + 5, option.rect.height() - 30, -5, -5);
    painter->setBrush(QColor(200, 200, 200));
    painter->drawRect(buttonRect2);
    painter->drawText(buttonRect2, Qt::AlignCenter, "Revoke Zero");

    painter->restore();
}

QSize CardDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(200, 100);
}

bool CardDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

        QRect checkBoxRect = option.rect;
        checkBoxRect.setLeft(option.rect.right() - 30);
        checkBoxRect.setWidth(20);

        if (checkBoxRect.contains(mouseEvent->pos())) {
            Qt::CheckState newState = (index.data(Qt::CheckStateRole).toBool()) ? Qt::Unchecked : Qt::Checked;
            model->setData(index, newState, Qt::CheckStateRole);
            return true;
        }

        QRect firstButtonRect = option.rect.adjusted(5, option.rect.height() - 30, -option.rect.width() / 2, -5);
        if (firstButtonRect.contains(mouseEvent->pos())) {
            emit zeroButtonClicked(index);
            return true;
        }

        QRect secondButtonRect = option.rect.adjusted(option.rect.width() / 2 + 5, option.rect.height() - 30, -5, -5);
        if (secondButtonRect.contains(mouseEvent->pos())) {
            emit restoreButtonClicked(index);
            return true;
        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
