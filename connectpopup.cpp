#include "connectpopup.h"
#include "ui_connectpopup.h"
#include "loadcell.h"
#include "globals.h"
#include <QMessageBox>
#include <QTimer>
#include <QDebug>

connectPopup::connectPopup(QTimer *timer, MainWindow *mainWindow, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::connectPopup),
    loadCell(nullptr),
    pointSet(false),
    lastSetPointIndex(-1),
    globalTimer(timer)
{
    ui->setupUi(this);
    ui->nextPointButton->setEnabled(false);
    ui->calibrateButton->setEnabled(false);
    ui->setPointButton->setEnabled(false);
}

connectPopup::~connectPopup()
{
}

void connectPopup::on_connectButton_clicked()
{
    int canId = ui->CANIdbox->value();
    uint32_t modifiedCanId = (0x06 << 8) | (canId & 0xFF);
    loadCell = new LoadCell(modifiedCanId, baudRate, globalTimer, this);

    QTimer::singleShot(10, [this]() {
        loadCell->Config_Init();
    });

    connect(loadCell, &LoadCell::ADCValueChanged, this, [this](double ADCValue) {
        ui->ADCValueLabel->setText(QString("ADC: %1").arg(ADCValue));
    });

    QTimer::singleShot(10, [this]() {
        loadCell->startSimulation();
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
    int colonIndex = adcText.indexOf(':');
    if (colonIndex != -1) {
        QString numberText = adcText.mid(colonIndex + 1).trimmed();
        bool ok;
        double currentADCValue = numberText.toDouble(&ok);
        double currentWeightValue = weightValue.toDouble(&ok);

        if (ok) {
            QString unit = ui->unitBox->currentText();
            if (unit == "kg") {
                currentWeightValue *= 1000.0;
            } else if (unit == "lb") {
                currentWeightValue *= 453.592;
            }

            currentWeightValue *= SCALING_FACTOR;

            if (lastSetPointIndex >= 0 && ui->nextPointButton->isEnabled()) {
                ADCPoints[lastSetPointIndex] = currentADCValue;
                weightPoints[lastSetPointIndex] = currentWeightValue;
            } else {
                ADCPoints.append(currentADCValue);
                weightPoints.append(currentWeightValue);
                lastSetPointIndex = ADCPoints.size() - 1;
            }
        }

        ui->nextPointButton->setEnabled(true);
        ui->calibrateButton->setEnabled(ADCPoints.size() >= 2);

        std::vector<uint8_t> calibrationData = {0x22, 0x0E, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00};
        size_t pointIndex = weightPoints.size() - 1;
        calibrationData[1] = 0x0E + pointIndex;
        unsigned int value = static_cast<unsigned int>(weightPoints[pointIndex]);
        std::vector<uint8_t> littleEndianBytes = decimalToLittleEndianBytes(value);
        std::copy(littleEndianBytes.begin(), littleEndianBytes.end(), calibrationData.end() - 4);
        manualReadInstance->SendMessages(calibrationData, 0x01);
        if (ADCPoints.size() == 1){
            loadCell->setZeroPoint(0);
        }
        pointSet = true;
    }
}

void connectPopup::on_nextPointButton_clicked()
{
    if (!pointSet) {
        return;
    }

    QString labelText = ui->currentCountNumberLabel->text();
    int colonIndex = labelText.indexOf(':');
    QString numberText;
    if (colonIndex != -1) {
        numberText = labelText.mid(colonIndex + 1).trimmed();
    } else {
        numberText = labelText.trimmed();
    }

    bool ok;
    int currentCount = numberText.toInt(&ok);
    if (!ok) {
        currentCount = 0;
    }

    currentCount++;
    QString message = QString("Current Points: %1").arg(currentCount);
    ui->currentCountNumberLabel->setText(message);

    QStringList ADCPointstrings;
    for (double point : ADCPoints) {
        ADCPointstrings << QString::number(point);
    }

    QString ADCPointsText = ADCPointstrings.join(", ");
    ui->nextPointButton->setEnabled(false);
    ui->calibrateButton->setEnabled(ADCPoints.size() >= 2);
}

void connectPopup::on_calibrateButton_clicked()
{
    std::vector<uint8_t> setCalibrationPointNumber = { 0x22, 0x04, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 };
    ManualRead* manualReadInstance = loadCell->getManualRead();
    size_t weightPointsSize = weightPoints.size();
    setCalibrationPointNumber[4] = (setCalibrationPointNumber[4] + static_cast<uint8_t>(weightPointsSize)) % 256;
    manualReadInstance->SendMessages(setCalibrationPointNumber, 01);
    loadCell->disconnectUpdateADCValue();
    loadCell->connectUpdateWeight();
    emit loadCellConnected(loadCell);
    loadcellConnected = true;
    loadCell->setADCPoints(ADCPoints);
    loadCell->setWeightPoints(weightPoints);
    this->close();
}

std::vector<BYTE> connectPopup::decimalToLittleEndianBytes(unsigned int decimalValue) {
    std::vector<BYTE> bytes;
    for (int i = 0; i < sizeof(decimalValue); ++i) {
        BYTE byte = (decimalValue >> (i * 8)) & 0xFF;
        bytes.push_back(byte);
    }
    return bytes;
}

void connectPopup::printByteVector(const std::vector<BYTE>& bytes) {
    QString hexString;
    for (BYTE byte : bytes) {
        hexString.append(QString::number(byte, 16).rightJustified(2, '0')).append(" ");
    }
    qDebug() << "Bytes (hex):" << hexString.trimmed();
}
