#ifndef CONNECTPOPUP_H
#define CONNECTPOPUP_H

#include "loadcell.h"
#include "utility.h"
#include <QDialog>
#include <vector>
#include <QVector>
#include <QTimer>  // Include QTimer
#include "mainwindow.h"
#include <QMutex>

namespace Ui {
class connectPopup;
}

class connectPopup : public QDialog
{
    Q_OBJECT

public:
    explicit connectPopup(QTimer *timer, MainWindow *mainWindow, QWidget *parent = nullptr);
    ~connectPopup();

private slots:
    void on_connectButton_clicked();
    void on_setPointButton_clicked();
    void on_nextPointButton_clicked();
    void on_calibrateButton_clicked();


signals:
    void loadCellConnected(LoadCell *loadCell);

private:
    MainWindow *mainWindow;  // Store a pointer to MainWindow
    QMutex loadCellsMutex;  // Mutex to protect access to loadCells
    Ui::connectPopup *ui;
    int canId;  // CAN ID member
    int baudRate;
    bool loadcellConnected = false;
    QString canInterface;
    QString mode;
    QVector<int> ADCPoints;
    QVector<double> weightPoints;
    bool pointSet;  // Track if a point has been set
    QTimer *globalTimer;  // Pointer to the global timer

    LoadCell *loadCell;
    int lastSetPointIndex;
    std::vector<std::byte> littleEndianBytes;

    const TPCANHandle PcanHandle = PCAN_USBBUS1;
    const TPCANBaudrate Bitrate = PCAN_BAUD_1M;

    std::vector<BYTE> decimalToLittleEndianBytes(unsigned int decimalValue);
    void printByteVector(const std::vector<BYTE>& bytes);
    void calibrate();
    void updateConfiguration();  // Moved to private section
};

#endif // CONNECTPOPUP_H
