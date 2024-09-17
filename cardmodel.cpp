#include "cardmodel.h"

CardModel::CardModel(QObject *parent)
    : QAbstractListModel(parent), weightUnit(Grams) // Initialize weight unit to Grams
{
}

int CardModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return cards.count();
}

QVariant CardModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= cards.count())
        return QVariant();

    const Card &card = cards[index.row()];

    if (role == Qt::DisplayRole) {
        QStringList displayData = card.labels;
        QString weightString;
        switch (weightUnit) {
        case Grams:
            weightString = QString("Current Weight: %1 g").arg(card.weight);
            break;
        case Kilograms:
            weightString = QString("Current Weight: %1 kg").arg(card.weight / 1000.0);
            break;
        case Pounds:
            weightString = QString("Current Weight: %1 lb").arg(card.weight * 0.00220462);
            break;
        }
        displayData.append(weightString);
        return displayData;
    } else if (role == Qt::CheckStateRole) { // Return toggle state
        return card.isChecked ? Qt::Checked : Qt::Unchecked;
    } else if (role == Qt::DecorationRole) {
        return card.color;
    }

    return QVariant();
}

bool CardModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole) {
        cards[index.row()].isChecked = value.toBool();
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags CardModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
}

void CardModel::addCard(const QStringList &labels, double weight, const QColor &color)
{
    beginInsertRows(QModelIndex(), cards.count(), cards.count());
    cards.append({labels, weight, color, false}); // Default toggle to off
    endInsertRows();
}

void CardModel::removeCard(int row)
{
    if (row < 0 || row >= cards.count())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    cards.removeAt(row);
    endRemoveRows();
}

void CardModel::updateCard(int row, double weight)
{
    if (row < 0 || row >= cards.count())
        return;

    cards[row].weight = weight;
    emit dataChanged(index(row), index(row));
}

QString CardModel::cardName(int index) const
{
    if (index >= 0 && index < cards.size()) {
        return cards[index].labels.join(", ");
    }
    return QString();
}

void CardModel::setWeightUnit(WeightUnit unit)
{
    weightUnit = unit;
    emit dataChanged(index(0), index(cards.count() - 1)); // Notify views of the unit change
}

CardModel::WeightUnit CardModel::getWeightUnit() const
{
    return weightUnit;
}
