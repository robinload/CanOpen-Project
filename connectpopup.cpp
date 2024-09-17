#include "connectpopup.h"
#include "ui_connectpopup.h"
#include "loadcell.h"
#include "globals.h"
#include <QMessageBox>
#include <QTimer>
#include <QDebug>


connectPopup::connectPopup(QTimer *timer,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::connectPopup),
    loadCell(nullptr),
    pointSet(false),  // Initialize pointSet to false
    lastSetPointIndex(-1),
    globalTimer(timer)
{
    ui->setupUi(this);

    // Disable the Next Point button initially
    ui->nextPointButton->setEnabled(false);
    ui->calibrateButton->setEnabled(false);
    ui->setPointButton->setEnabled(false);
    // this->setAttribute(Qt::WA_DeleteOnClose);
    // updateConfiguration();
}

connectPopup::~connectPopup()
{
}

void connectPopup::on_connectButton_clicked()
{
    int canId = ui->CANIdbox->value();  // Read CAN ID from the box

    // Create a new LoadCell instance and store it in the member variable
    loadCell = new LoadCell(canId, baudRate, globalTimer, this);

    QTimer::singleShot(10, [this]() {
        loadCell->Config_Init(); // Start the simulation after 10 ms
    });

    connect(loadCell, &LoadCell::ADCValueChanged, this, [this](double ADCValue) {
        ui->ADCValueLabel->setText(QString("ADC: %1").arg(ADCValue));
    });



    QTimer::singleShot(10, [this]() {
        loadCell->startSimulation(); // Start the simulation after 10 ms
    });
    ui->setPointButton->setEnabled(true);
}

void connectPopup::on_setPointButton_clicked()
{
    std::vector<uint8_t> enablePoints = { 0x2F, 0x25, 0x20, 0x03, 0x01, 0x00, 0x00, 0x00 };
    ManualRead* manualReadInstance = loadCell->getManualRead();
    manualReadInstance->SendMessages(enablePoints, 0x01);

    QString adcText = ui->ADCValueLabel->text();
    QString weightValue = ui->weightBox->text();

    // Extract the ADC value after the colon
    int colonIndex = adcText.indexOf(':');
    if (colonIndex != -1) {
        QString numberText = adcText.mid(colonIndex + 1).trimmed();
        bool ok;
        double currentADCValue = numberText.toDouble(&ok);
        double currentWeightValue = weightValue.toDouble(&ok);

        if (ok) {
            // Convert weight to grams based on the selected unit
            QString unit = ui->unitBox->currentText();
            if (unit == "kg") {
                currentWeightValue *= 1000.0; // Convert kg to grams
            } else if (unit == "lb") {
                currentWeightValue *= 453.592; // Convert lb to grams
            }

            // Scale by local factor to handle decimal points
            currentWeightValue *= SCALING_FACTOR;

            // Check if the point can be added
            if (lastSetPointIndex >= 0 && ui->nextPointButton->isEnabled()) {
                // Update the existing point
                ADCPoints[lastSetPointIndex] = currentADCValue;
                weightPoints[lastSetPointIndex] = currentWeightValue;
            } else {
                // Add a new point
                ADCPoints.append(currentADCValue);
                weightPoints.append(currentWeightValue);
                lastSetPointIndex = ADCPoints.size() - 1;

                }
            }

            // Enable buttons as needed
            ui->nextPointButton->setEnabled(true);
            ui->calibrateButton->setEnabled(ADCPoints.size() >= 2);

            std::vector<uint8_t> calibrationData = {0x22, 0x0E, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00};

            // Check how many points are in weightPoints
            size_t pointIndex = weightPoints.size() - 1;

            // Adjust the second BYTE based on the number of points
            calibrationData[1] = 0x0E + pointIndex;

            // Get the value for the current point
            unsigned int value = static_cast<unsigned int>(weightPoints[pointIndex]);

            // Convert the value to little-endian bytes
            std::vector<uint8_t> littleEndianBytes = decimalToLittleEndianBytes(value);

            // Replace the last 4 bytes with the calculated little-endian value
            std::copy(littleEndianBytes.begin(), littleEndianBytes.end(), calibrationData.end() - 4);

            // Send the modified calibration data
            manualReadInstance->SendMessages(calibrationData, 0x01);
            if (ADCPoints.size()==1){
                loadCell->setZeroPoint(0);}
            pointSet = true;
        }
    }

void connectPopup::on_nextPointButton_clicked()
{
    if (!pointSet) {
        // If no point has been set, do nothing
        return;
    }

    // Retrieve the text from the label
    QString labelText = ui->currentCountNumberLabel->text();

    // Find the position of the colon and extract the substring after it
    int colonIndex = labelText.indexOf(':');
    QString numberText;
    if (colonIndex != -1) {
        numberText = labelText.mid(colonIndex + 1).trimmed(); // Get the substring after the colon and trim whitespace
    } else {
        numberText = labelText.trimmed(); // No colon, use the entire text
    }

    bool ok;
    int currentCount = numberText.toInt(&ok); // Convert to int
    if (!ok) {
        qDebug() << "Failed to convert label text to int. Setting count to 0.";
        currentCount = 0;
    }

    qDebug() << "Current count before increment:" << currentCount;

    // Increment the count
    currentCount++;

    // Create a formatted string with the updated count
    QString message = QString("Current Points: %1").arg(currentCount);

    // Update the label with the new count
    ui->currentCountNumberLabel->setText(message);

    // Update the status label
    // ui->statusLabel->setText("Set points updated.");

    // Print current count after increment
    qDebug() << "Current count after increment:" << currentCount;

    QStringList ADCPointstrings;
    for (double point : ADCPoints) {
        ADCPointstrings << QString::number(point);
    }

    // Join the QStringList into a single string with comma separation
    QString ADCPointsText = ADCPointstrings.join(", ");

    // Print all set points in one line
    qDebug() << "Current Set Points:" << ADCPointsText;
    ui->nextPointButton->setEnabled(false);

    // Check if there are at least two points to enable the calibration button
    ui->calibrateButton->setEnabled(ADCPoints.size() >= 2);
}

void connectPopup::on_calibrateButton_clicked()
{
    std::vector<uint8_t> setCalibrationPointNumber = { 0x22, 0x04, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 };

    // Get the ManualRead instance
    ManualRead* manualReadInstance = loadCell->getManualRead();

    // Get the size of weightPoints
    size_t weightPointsSize = weightPoints.size();

    setCalibrationPointNumber[4] = (setCalibrationPointNumber[4] + static_cast<uint8_t>(weightPointsSize)) % 256;
    manualReadInstance->SendMessages(setCalibrationPointNumber, 01);
    loadCell->disconnectUpdateADCValue();
    loadCell->connectUpdateWeight();
    emit loadCellConnected(loadCell);
    loadcellConnected=true;
    loadCell->setADCPoints(ADCPoints);
    loadCell->setWeightPoints(weightPoints);
    this->close();
}


std::vector<BYTE> connectPopup::decimalToLittleEndianBytes(unsigned int decimalValue) {
    std::vector<BYTE> bytes;

    // Convert to little-endian byte order
    for (int i = 0; i < sizeof(decimalValue); ++i) {
        BYTE byte = (decimalValue >> (i * 8)) & 0xFF;
        bytes.push_back(byte);
    }
    // Print the bytes before returning
    std::cout << "Little-endian bytes: ";
    for (BYTE byte : bytes) {
        std::cout << std::hex << static_cast<int>(byte) << ' ';
    }
    std::cout << std::endl;
    return bytes;
}
void connectPopup::printByteVector(const std::vector<BYTE>& bytes) {
    QString hexString;
    for (BYTE byte : bytes) {
        hexString.append(QString::number(byte, 16).rightJustified(2, '0')).append(" ");
    }
    qDebug() << "Bytes (hex):" << hexString.trimmed();
}


