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

    // Existing painting code for background, border, labels, and checkbox
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

    // Drawing the first button
    QRect buttonRect1 = option.rect.adjusted(5, option.rect.height() - 30, -option.rect.width() / 2 - 5, -5); // Adjusted position for the first button
    painter->setBrush(QColor(200, 200, 200)); // Button color
    painter->drawRect(buttonRect1);
    painter->drawText(buttonRect1, Qt::AlignCenter, "Zero");

    // Drawing the second button
    QRect buttonRect2 = option.rect.adjusted(option.rect.width() / 2 + 5, option.rect.height() - 30, -5, -5); // Adjusted position for the second button
    painter->setBrush(QColor(200, 200, 200)); // Button color
    painter->drawRect(buttonRect2);
    painter->drawText(buttonRect2, Qt::AlignCenter, "Revoke Zero");

    painter->restore();
}


QSize CardDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(200, 100);
}bool CardDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

        QRect checkBoxRect = option.rect;
        checkBoxRect.setLeft(option.rect.right() - 30); // Adjust to position checkbox
        checkBoxRect.setWidth(20);

        // Check if the checkbox is clicked
        if (checkBoxRect.contains(mouseEvent->pos())) {
            Qt::CheckState newState = (index.data(Qt::CheckStateRole).toBool()) ? Qt::Unchecked : Qt::Checked;
            model->setData(index, newState, Qt::CheckStateRole);
            return true; // Event handled
        }

        // Check if the first button is clicked
        QRect firstButtonRect = option.rect.adjusted(5, option.rect.height() - 30, -option.rect.width() / 2, -5);
        if (firstButtonRect.contains(mouseEvent->pos())) {
            emit zeroButtonClicked(index); // Emit signal when the first button is clicked
            return true; // Event handled
        }

        // Check if the second button is clicked
        QRect secondButtonRect = option.rect.adjusted(option.rect.width() / 2 + 5, option.rect.height() - 30, -5, -5);
        if (secondButtonRect.contains(mouseEvent->pos())) {
            emit restoreButtonClicked(index); // Emit signal when the second button is clicked
            return true; // Event handled
        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
