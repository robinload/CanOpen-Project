#include <QTimer>
#include "loadcell.h"
#include "globals.h"
#include <QDebug>
#include <QFileDialog>    // For opening the file dialog
#include <QFile>
#include <QMessageBox>

LoadCell::LoadCell(int canId, int baudRate, QTimer *globalTimer, QObject *parent)
    : QObject(parent),
    canId(canId),
    baudRate(baudRate),
    globalTimer(globalTimer), // Initialize globalTimer
    weight(0.0),
    ADCValue(0.0),
    manualRead(new ManualRead())
{
    // Connect the global timer's timeout signal to the relevant slots
    weightPoints = QVector<double>();  // Initializes an empty QVector for weightPoints
    ADCPoints = QVector<int>();     // Initializes an empty QVector for setPoints
    connect(globalTimer, &QTimer::timeout, this, &LoadCell::updateADCValue);
    // connect(globalTimer, &QTimer::timeout, this, &LoadCell::updateWeight);
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
        globalTimer->start(10); // Update every 10 ms
    }
}

int LoadCell::getCanId() const
{
    return canId; // Return the CAN ID
}

void LoadCell::setIndex(int newIndex)
{
    index = newIndex;
}

double LoadCell::currentWeight() const
{
    return weight; // Return the current weight
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
    auto [status,responseId, rxCommand] = manualRead->ReadMessages();

    if (status == PCAN_ERROR_OK && rxCommand.size() >= 6 && rxCommand[1] == requestData[1])
    {
        int decimalValue = (rxCommand[7] << 24) | (rxCommand[6] << 16) | (rxCommand[5] << 8) | rxCommand[4];

        // Check if the sign bit (23rd bit) is set
        if (decimalValue & 0x800000)
        {
            // Convert to a negative value using two's complement
            decimalValue |= 0xFF000000; // Set the upper bits to 1
        }

        // Convert to double and apply scaling factor
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
    auto [status,responseID, rxCommand] = manualRead->ReadMessages();

    if (rxCommand.size() >= 6 && rxCommand[1] == data[1] && rxCommand[2] == data[2])
    {
        int decimalValue = (rxCommand[7] << 24) |(rxCommand[6] << 16) | (rxCommand[5] << 8) | rxCommand[4];
        emit ADCValueChanged(decimalValue);
    }
}

void LoadCell::setADCSamplingSpeed(uint32_t sample_speed) {
    std::vector<BYTE> data = { 0x22, 0x17, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 };
    data[4] = static_cast<BYTE>(sample_speed);

    // Debug output
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

    // Debug output
    QString dataString;
    for (BYTE byte : data) {
        dataString += QString::asprintf("%02X ", byte);
    }
    qDebug() << "Function:" << Q_FUNC_INFO << "Data to send:" << dataString.trimmed();

    manualRead->SendMessages(data, canId);
}

void LoadCell::setFilteringLevel(uint8_t level) {
    // Ensure the level is between 1 and 5
    if (level < 1 || level > 5) {
        level = 1; // Default value or handle as needed
    }

    std::vector<BYTE> data = { 0x22, 0x19, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 };
    data[4] = level;

    // Debug output
    QString dataString;
    for (BYTE byte : data) {
        dataString += QString::asprintf("%02X ", byte);
    }
    qDebug() << "Function:" << Q_FUNC_INFO << "Data to send:" << dataString.trimmed();

    manualRead->SendMessages(data, canId);
}

void LoadCell::setFilteringBandwidth(uint32_t bandwidth) {
    std::vector<BYTE> data = { 0x22, 0x1A, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 };
    data[4] = static_cast<BYTE>(bandwidth & 0xFF);        // Least significant byte
    data[5] = static_cast<BYTE>((bandwidth >> 8) & 0xFF); // Second byte
    data[6] = static_cast<BYTE>((bandwidth >> 16) & 0xFF);// Third byte
    data[7] = static_cast<BYTE>((bandwidth >> 24) & 0xFF);// Most significant byte

    // Debug output
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

    // Debug output
    QString dataString;
    for (BYTE byte : data) {
        dataString += QString::asprintf("%02X ", byte);
    }
    qDebug() << "Function:" << Q_FUNC_INFO << "Data to send:" << dataString.trimmed();

    manualRead->SendMessages(data, canId);
}

void LoadCell::setAutoZeroTrackingBandwidth(uint32_t bandwidth) {
    std::vector<BYTE> data = { 0x22, 0x1C, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 };
    data[4] = static_cast<BYTE>(bandwidth & 0xFF);        // Least significant byte
    data[5] = static_cast<BYTE>((bandwidth >> 8) & 0xFF); // Second byte
    data[6] = static_cast<BYTE>((bandwidth >> 16) & 0xFF);// Third byte
    data[7] = static_cast<BYTE>((bandwidth >> 24) & 0xFF);// Most significant byte

    // Debug output
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

    // Debug output
    QString dataString;
    for (BYTE byte : data) {
        dataString += QString::asprintf("%02X ", byte);
    }
    qDebug() << "Function:" << Q_FUNC_INFO << "Data to send:" << dataString.trimmed();

    manualRead->SendMessages(data, canId);
}


void LoadCell::setZeroPoint(uint32_t zeroPoint) {
    // Initialize the data array with the provided values
    std::vector<BYTE> data = { 0x22, 0x03, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 };

    // Set the zero point value in the data array
    data[4] = static_cast<BYTE>(zeroPoint & 0xFF);
    data[5] = static_cast<BYTE>((zeroPoint >> 8) & 0xFF);
    data[6] = static_cast<BYTE>((zeroPoint >> 16) & 0xFF);
    data[7] = static_cast<BYTE>((zeroPoint >> 24) & 0xFF);

    // Debug output
    QString dataString;
    for (BYTE byte : data) {
        dataString += QString::asprintf("%02X ", byte);
    }
    qDebug() << "Function:" << Q_FUNC_INFO << "Data to send:" << dataString.trimmed();

    // Send the message using the CAN interface
    manualRead->SendMessages(data, canId);
}

void LoadCell::setStabilityBandwidth(uint32_t bandwidth) {
    std::vector<BYTE> data = { 0x22, 0x1E, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 };
    data[4] = static_cast<BYTE>(bandwidth & 0xFF);        // Least significant byte
    data[5] = static_cast<BYTE>((bandwidth >> 8) & 0xFF); // Second byte
    data[6] = static_cast<BYTE>((bandwidth >> 16) & 0xFF);// Third byte
    data[7] = static_cast<BYTE>((bandwidth >> 24) & 0xFF);// Most significant byte

    // Debug output
    QString dataString;
    for (BYTE byte : data) {
        dataString += QString::asprintf("%02X ", byte);
    }
    qDebug() << "Function:" << Q_FUNC_INFO << "Data to send:" << dataString.trimmed();

    manualRead->SendMessages(data, canId);
}

void LoadCell::saveCalibrationData() {
    // Open a file dialog for the user to choose the file location
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save Calibration Data", "", "CSV Files (*.csv)");

    // Check if the user selected a file
    if (fileName.isEmpty()) {
        qDebug() << "No file selected for saving calibration data.";
        return;
    }

    // Ensure the file has a .csv extension
    if (!fileName.endsWith(".csv", Qt::CaseInsensitive)) {
        fileName += ".csv";
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Error", "Failed to open file for writing.");
        return;
    }

    QTextStream out(&file);

    // Write the CSV header
    out << "Point,ADC,Weight (grams)\n";

    // Write each calibration point as a row in the CSV file
    for (size_t i = 0; i < ADCPoints.size(); ++i) {
        out << (i + 1) << "," << ADCPoints[i] << "," << weightPoints[i] << "\n";
    }

    file.close();

    QMessageBox::information(nullptr, "Success", "Calibration data saved successfully.");
}



void LoadCell::Config_Init(){
    qDebug()<<"Inside Config Init";
    uint32_t defaultSampleSpeed = 100; // Example default value
    bool defaultFIRFiltering = true; // Example default value
    uint8_t defaultFilteringLevel = 2; // Example default value
    uint32_t defaultFilteringBandwidth = 1000; // Example default value
    uint16_t defaultAutoZeroTrackingTime = 2; // Example default value
    uint32_t defaultAutoZeroTrackingBandwidth = 1000; // Example default value
    uint32_t defaultStabilityDeterminationTime = 5; // Example default value
    uint32_t defaultStabilityBandwidth = 1000; // Example default value

    // Call each configuration function with default values
    setADCSamplingSpeed(defaultSampleSpeed);
    setFIRFiltering(defaultFIRFiltering);
    setFilteringLevel(defaultFilteringLevel);
    setFilteringBandwidth(defaultFilteringBandwidth);
    setAutoZeroTrackingTime(defaultAutoZeroTrackingTime);
    setAutoZeroTrackingBandwidth(defaultAutoZeroTrackingBandwidth);
    setStabilityDeterminationTime(defaultStabilityDeterminationTime);
    setStabilityBandwidth(defaultStabilityBandwidth);
}











