#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QListView>
#include "loadcell.h"
#include "cardmodel.h"
#include "commanddialog.h"
#include <QDateTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QList<LoadCell*> getLoadCells() const;

private slots:
    void realtimeDataSlot();
    void removeCard();
    void openConnectPopup();
    void openSamplingPage();
    void addLoadCellCard(LoadCell *loadCell);
    void toggleAutoScale(bool checked);
    void updatePlotUnits(); // Slot to handle updates to the plot based on the selected unit
    void startSampling();
    void stopSampling();
    void updateYRange();
    void zeroButtonClicked(const QModelIndex &index);
    void restoreButtonClicked(const QModelIndex &index);
    LoadCell* getSelectedLoadCell() const;

private:
    Ui::MainWindow *ui;
    QTimer *globalTimer;  // Declare globalTimer as a member
    QListView *listView;
    ManualRead *manualRead; // Pointer to the ManualRead instance
    CardModel *cardModel;
    QDateTime startTime; // New member for tracking the start time
    QList<LoadCell*> loadCells;
    QWidget *calibrationPopupWidget; // Pointer to the popup widget
    QList<QColor> usedColors;
    QColor getAvailableColor();
    void CANBusConnect();
    void CANBusDisconnect();
    void openFilterConfig();
    void saveCalibrationSetting();
    void loadCalibrationSetting();
    bool autoScaleEnabled = true; // Member variable to track the state of auto scaling
    bool isSampling;
    void openAdvancedDialog();
    double convertWeightToSelectedUnit(double weight); // Converts weight based on the selected unit
    const TPCANHandle PcanHandle = PCAN_USBBUS1;
    const TPCANBaudrate Bitrate = PCAN_BAUD_1M;
    bool isCANInitialized = false;
};

#endif // MAINWINDOW_H
