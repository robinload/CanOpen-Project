#ifndef CARDMODEL_H
#define CARDMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QColor> // Add this include for QColor

class CardModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit CardModel(QObject *parent = nullptr);
    QString cardName(int index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void addCard(const QStringList &labels, double weight = 0.0, const QColor &color = QColor());
    void removeCard(int row);
    void updateCard(int row, double weight);
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    enum WeightUnit {
        Grams,
        Kilograms,
        Pounds
    };

    void setWeightUnit(WeightUnit unit);
    WeightUnit getWeightUnit() const;

private:
    struct Card {
        QStringList labels;
        double weight;
        QColor color;
        bool isChecked; // Toggle state
    };

    QList<Card> cards;
    WeightUnit weightUnit; // Current weight unit
};

#endif // CARDMODEL_H
