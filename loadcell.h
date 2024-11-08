#ifndef LOADCELL_H
#define LOADCELL_H

#include <QObject>
#include <QVector>
#include <QPair>
#include "utility.h"

class QTimer; // Forward declaration



class LoadCell : public QObject
{
    Q_OBJECT

public:
    explicit LoadCell(int canId, int baudRate, QTimer *globalTimer, QObject *parent);
    ~LoadCell();
    ManualRead* getManualRead() const; // Getter method to access ManualRead instance
    void startSimulation();
    void setIndex(int newIndex);
    double currentWeight() const;
    void calibrate();
    int getCanId() const; // Add this line to the public section
    unsigned int decimalToHexadecimal(int decimalNumber);

    void setADCPoints(const QVector<int>& points);
    void setWeightPoints(const QVector<double>& points);

    // Getter methods
    const QVector<int>& getADCPoints() const;
    const QVector<double>& getWeightPoints() const;

    void setADCSamplingSpeed(uint32_t sample_speed);
    void setFIRFiltering(bool enable);
    void setFilteringLevel(uint8_t level);
    void setFilteringBandwidth(uint32_t bandwidth);
    void setAutoZeroTrackingTime(uint16_t timeMs);
    void setAutoZeroTrackingBandwidth(uint32_t bandwidth);
    void setStabilityDeterminationTime(uint32_t time);
    void setStabilityBandwidth(uint32_t bandwidth);
    void Config_Init();
    void setZeroPoint(uint32_t zeroPoint);
    void saveCalibrationData();
    void waitForSpecificCANID(uint32_t targetID);
    void connectUpdateADCValue();
    void disconnectUpdateADCValue();
    void connectUpdateWeight();
    void disconnectUpdateWeight();

signals:
    void weightChanged(double newWeight);
    void ADCValueChanged(int newADCValue);

private slots:
    void updateADCValue();
    void updateWeight();

private:
    HANDLE hEvent;  // Handle for CAN events
    int index;
    int canId; // Add this member
    double weight;
    double ADCValue;
    QVector<double> weightPoints;  // Stores weight calibration points
    QVector<int> ADCPoints;
    QTimer *globalTimer; // Pointer to the global timer
    int expectedReturnId;

    double adcValue;
    std::vector<BYTE> rxCommand;
    int baudRate;
    ManualRead *manualRead;  // Pointer to ManualRead instance

};

#endif // LOADCELL_H
