#include <QTimer>
#include "loadcell.h"
#include "globals.h"
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <sstream>

LoadCell::LoadCell(int canId, int baudRate, QTimer *globalTimer, QObject *parent)
    : QObject(parent),
    canId(canId),
    baudRate(baudRate),
    globalTimer(globalTimer),
    weight(0.0),
    ADCValue(0.0),
    manualRead(new ManualRead())
{
    weightPoints = QVector<double>();
    ADCPoints = QVector<int>();
    connect(globalTimer, &QTimer::timeout, this, &LoadCell::updateADCValue);
    expectedReturnId = canId - 128;
}

LoadCell::~LoadCell()
{
    disconnectUpdateADCValue();
    disconnectUpdateWeight();
    delete manualRead;
}

void LoadCell::disconnectUpdateADCValue()
{
    disconnect(globalTimer, &QTimer::timeout, this, &LoadCell::updateADCValue);
}

void LoadCell::connectUpdateADCValue()
{
    connect(globalTimer, &QTimer::timeout, this, &LoadCell::updateADCValue);
}

void LoadCell::disconnectUpdateWeight()
{
    disconnect(globalTimer, &QTimer::timeout, this, &LoadCell::updateWeight);
}

void LoadCell::connectUpdateWeight()
{
    connect(globalTimer, &QTimer::timeout, this, &LoadCell::updateWeight);
}

ManualRead* LoadCell::getManualRead() const
{
    return manualRead;
}

void LoadCell::startSimulation()
{
    if (globalTimer) {
        globalTimer->start(10);
    }
}

int LoadCell::getCanId() const
{
    return canId;
}

void LoadCell::setIndex(int newIndex)
{
    index = newIndex;
}

double LoadCell::currentWeight() const
{
    return weight;
}

unsigned int LoadCell::decimalToHexadecimal(int decimalNumber)
{
    return static_cast<unsigned int>(decimalNumber);
}

void LoadCell::setADCPoints(const QVector<int>& points) {
    ADCPoints = points;
}

const QVector<int>& LoadCell::getADCPoints() const {
    return ADCPoints;
}

void LoadCell::setWeightPoints(const QVector<double>& points) {
    weightPoints = points;
}

const QVector<double>& LoadCell::getWeightPoints() const {
    return weightPoints;
}




void LoadCell::updateWeight()
{
    const std::vector<BYTE> data = { 0x40, 0x01, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 };
    std::vector<BYTE> requestData = data;
    requestData[1] = static_cast<BYTE>(canId & 0xFF);
    manualRead->SendMessages(requestData, canId);
    std::tuple<TPCANStatus, UINT32, std::vector<BYTE>> result = manualRead->ReadMessage();
    TPCANStatus status = std::get<0>(result);
    UINT32 responseId = std::get<1>(result);
    std::vector<BYTE> rxCommand = std::get<2>(result);
    if (responseId != expectedReturnId) {
        return;
    }
    if (status == PCAN_ERROR_OK && rxCommand.size() >= 6 && rxCommand[1] == requestData[1])
    {
        int decimalValue = (rxCommand[7] << 24) | (rxCommand[6] << 16) | (rxCommand[5] << 8) | rxCommand[4];
        if (decimalValue & 0x800000)
        {
            decimalValue |= 0xFF000000;
        }
        double weightInDouble = static_cast<double>(decimalValue);
        weightInDouble = weightInDouble / SCALING_FACTOR;
        weight = weightInDouble;
        emit weightChanged(weightInDouble);
    }
}


void LoadCell::updateADCValue()
{
    const std::vector<BYTE> data = { 0x40, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 };
    manualRead->SendMessages(data, canId);
    auto [status, responseID, rxCommand] = manualRead->ReadMessage();
    if (rxCommand.size() >= 4 && rxCommand[0] == 0x43 && rxCommand[1] == 0x00 &&
        rxCommand[2] == 0x20 && rxCommand[3] == 0x00)
    {
        if (rxCommand.size() >= 8 && rxCommand[1] == data[1] && rxCommand[2] == data[2])
        {
            int decimalValue = (rxCommand[7] << 24) | (rxCommand[6] << 16) |
                               (rxCommand[5] << 8)  | rxCommand[4];
            emit ADCValueChanged(decimalValue);
        }
    }
    else
    {
        return;
    }
}

void LoadCell::setADCSamplingSpeed(uint32_t sample_speed) {
    std::vector<BYTE> data = { 0x22, 0x17, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 };
    data[4] = static_cast<BYTE>(sample_speed);
    QString dataString;
    for (BYTE byte : data) {
        dataString += QString::asprintf("%02X ", byte);
    }
    qDebug() << "Function:" << Q_FUNC_INFO << "Data to send:" << dataString.trimmed();
    manualRead->SendMessages(data, canId);
}

void LoadCell::setFIRFiltering(bool enable) {
    std::vector<BYTE> data = { 0x22, 0x18, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 };
    data[4] = enable ? 0x01 : 0x00;
    QString dataString;
    for (BYTE byte : data) {
        dataString += QString::asprintf("%02X ", byte);
    }
    qDebug() << "Function:" << Q_FUNC_INFO << "Data to send:" << dataString.trimmed();
    manualRead->SendMessages(data, canId);
}

void LoadCell::setFilteringLevel(uint8_t level) {
    if (level < 1 || level > 5) {
        level = 1;
    }
    std::vector<BYTE> data = { 0x22, 0x19, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 };
    data[4] = level;
    QString dataString;
    for (BYTE byte : data) {
        dataString += QString::asprintf("%02X ", byte);
    }
    qDebug() << "Function:" << Q_FUNC_INFO << "Data to send:" << dataString.trimmed();
    manualRead->SendMessages(data, canId);
}

void LoadCell::setFilteringBandwidth(uint32_t bandwidth) {
    std::vector<BYTE> data = { 0x22, 0x1A, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 };
    data[4] = static_cast<BYTE>(bandwidth & 0xFF);
    data[5] = static_cast<BYTE>((bandwidth >> 8) & 0xFF);
    data[6] = static_cast<BYTE>((bandwidth >> 16) & 0xFF);
    data[7] = static_cast<BYTE>((bandwidth >> 24) & 0xFF);
    QString dataString;
    for (BYTE byte : data) {
        dataString += QString::asprintf("%02X ", byte);
    }
    qDebug() << "Function:" << Q_FUNC_INFO << "Data to send:" << dataString.trimmed();
    manualRead->SendMessages(data, canId);
}

void LoadCell::setAutoZeroTrackingTime(uint16_t timeMs) {
    std::vector<BYTE> data = { 0x22, 0x1B, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 };
    data[4] = static_cast<BYTE>(timeMs & 0xFF);
    data[5] = static_cast<BYTE>((timeMs >> 8) & 0xFF);
    QString dataString;
    for (BYTE byte : data) {
        dataString += QString::asprintf("%02X ", byte);
    }
    qDebug() << "Function:" << Q_FUNC_INFO << "Data to send:" << dataString.trimmed();
    manualRead->SendMessages(data, canId);
}

void LoadCell::setAutoZeroTrackingBandwidth(uint32_t bandwidth) {
    std::vector<BYTE> data = { 0x22, 0x1C, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 };
    data[4] = static_cast<BYTE>(bandwidth & 0xFF);
    data[5] = static_cast<BYTE>((bandwidth >> 8) & 0xFF);
    data[6] = static_cast<BYTE>((bandwidth >> 16) & 0xFF);
    data[7] = static_cast<BYTE>((bandwidth >> 24) & 0xFF);
    QString dataString;
    for (BYTE byte : data) {
        dataString += QString::asprintf("%02X ", byte);
    }
    qDebug() << "Function:" << Q_FUNC_INFO << "Data to send:" << dataString.trimmed();
    manualRead->SendMessages(data, canId);
}

void LoadCell::setStabilityDeterminationTime(uint32_t time) {
    std::vector<BYTE> data = { 0x22, 0x1D, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 };
    data[4] = static_cast<BYTE>(time & 0xFF);
    data[5] = static_cast<BYTE>((time >> 8) & 0xFF);
    data[6] = static_cast<BYTE>((time >> 16) & 0xFF);
    data[7] = static_cast<BYTE>((time >> 24) & 0xFF);
    QString dataString;
    for (BYTE byte : data) {
        dataString += QString::asprintf("%02X ", byte);
    }
    qDebug() << "Function:" << Q_FUNC_INFO << "Data to send:" << dataString.trimmed();
    manualRead->SendMessages(data, canId);
}

void LoadCell::setZeroPoint(uint32_t zeroPoint) {
    std::vector<BYTE> data = { 0x22, 0x03, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 };
    data[4] = static_cast<BYTE>(zeroPoint & 0xFF);
    data[5] = static_cast<BYTE>((zeroPoint >> 8) & 0xFF);
    data[6] = static_cast<BYTE>((zeroPoint >> 16) & 0xFF);
    data[7] = static_cast<BYTE>((zeroPoint >> 24) & 0xFF);
    QString dataString;
    for (BYTE byte : data) {
        dataString += QString::asprintf("%02X ", byte);
    }
    qDebug() << "Function:" << Q_FUNC_INFO << "Data to send:" << dataString.trimmed();
    manualRead->SendMessages(data, canId);
}

void LoadCell::setStabilityBandwidth(uint32_t bandwidth) {
    std::vector<BYTE> data = { 0x22, 0x1E, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 };
    data[4] = static_cast<BYTE>(bandwidth & 0xFF);
    data[5] = static_cast<BYTE>((bandwidth >> 8) & 0xFF);
    data[6] = static_cast<BYTE>((bandwidth >> 16) & 0xFF);
    data[7] = static_cast<BYTE>((bandwidth >> 24) & 0xFF);
    QString dataString;
    for (BYTE byte : data) {
        dataString += QString::asprintf("%02X ", byte);
    }
    qDebug() << "Function:" << Q_FUNC_INFO << "Data to send:" << dataString.trimmed();
    manualRead->SendMessages(data, canId);
}

void LoadCell::saveCalibrationData() {
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save Calibration Data", "", "CSV Files (*.csv)");
    if (fileName.isEmpty()) {
        qDebug() << "No file selected for saving calibration data.";
        return;
    }
    if (!fileName.endsWith(".csv", Qt::CaseInsensitive)) {
        fileName += ".csv";
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Error", "Failed to open file for writing.");
        return;
    }
    QTextStream out(&file);
    out << "Point,ADC,Weight (grams)\n";
    for (size_t i = 0; i < ADCPoints.size(); ++i) {
        out << (i + 1) << "," << ADCPoints[i] << "," << weightPoints[i] << "\n";
    }
    file.close();
    QMessageBox::information(nullptr, "Success", "Calibration data saved successfully.");
}

void LoadCell::Config_Init(){
    qDebug()<<"Inside Config Init";
    uint32_t defaultSampleSpeed = 100;
    bool defaultFIRFiltering = true;
    uint8_t defaultFilteringLevel = 2;
    uint32_t defaultFilteringBandwidth = 1000;
    uint16_t defaultAutoZeroTrackingTime = 2;
    uint32_t defaultAutoZeroTrackingBandwidth = 1000;
    uint32_t defaultStabilityDeterminationTime = 5;
    uint32_t defaultStabilityBandwidth = 1000;
    setADCSamplingSpeed(defaultSampleSpeed);
    setFIRFiltering(defaultFIRFiltering);
    setFilteringLevel(defaultFilteringLevel);
    setFilteringBandwidth(defaultFilteringBandwidth);
    setAutoZeroTrackingTime(defaultAutoZeroTrackingTime);
    setAutoZeroTrackingBandwidth(defaultAutoZeroTrackingBandwidth);
    setStabilityDeterminationTime(defaultStabilityDeterminationTime);
    setStabilityBandwidth(defaultStabilityBandwidth);
}
