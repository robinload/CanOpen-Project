#ifndef LOADCELL_MANAGER_H
#define LOADCELL_MANAGER_H

#include <QObject>
#include <QTimer>
#include <map>
#include "LoadCell.h"
#include "utility.h"

class ManualRead; // Forward declaration

class LoadCellManager : public QObject
{
    Q_OBJECT

public:
    explicit LoadCellManager(ManualRead* reader, QObject *parent = nullptr);
    ~LoadCellManager();

public slots:
    void updateAllLoadCells();
    void deleteLoadCell(int canId);

private slots:
    void onTimerTimeout();

private:
    void updateWeight(int canId);
    void updateADCValue(int canId);

    ManualRead* manualRead;
    QTimer* updateTimer;
    std::unordered_map<int, int> canIdToInternalIdMap; // Map CAN IDs to internal IDs
};

#endif // LOADCELLMANAGER_H
