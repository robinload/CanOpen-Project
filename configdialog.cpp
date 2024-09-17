#include "configdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

ConfigDialog::ConfigDialog(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Configure CAN Settings");

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Baud Rate
    QLabel *baudRateLabel = new QLabel("Baud Rate:", this);
    layout->addWidget(baudRateLabel);

    baudRateComboBox = new QComboBox(this);
    baudRateComboBox->addItems({"125000", "250000", "500000", "1000000"});
    layout->addWidget(baudRateComboBox);

    // CAN Interface
    QLabel *interfaceLabel = new QLabel("CAN Interface:", this);
    layout->addWidget(interfaceLabel);

    interfaceComboBox = new QComboBox(this);
    interfaceComboBox->addItems({"can0", "can1"});
    layout->addWidget(interfaceComboBox);

    // CAN Mode
    QLabel *modeLabel = new QLabel("CAN Mode:", this);
    layout->addWidget(modeLabel);

    modeComboBox = new QComboBox(this);
    modeComboBox->addItems({"Normal", "Listen-Only", "Loopback"});
    layout->addWidget(modeComboBox);

    QPushButton *okButton = new QPushButton("OK", this);
    layout->addWidget(okButton);

    connect(okButton, &QPushButton::clicked, this, &ConfigDialog::accept);

    // Load the previous settings
    loadSettings();
}

ConfigDialog::~ConfigDialog() {
}

int ConfigDialog::getBaudRate() const {
    return baudRateComboBox->currentText().toInt();  // Convert text to int
}

QString ConfigDialog::getInterface() const {
    return interfaceComboBox->currentText();
}

QString ConfigDialog::getMode() const {
    return modeComboBox->currentText();
}

void ConfigDialog::loadSettings() {
    QSettings settings("YourCompany", "YourApp");
    baudRateComboBox->setCurrentText(settings.value("baudRate", "250000").toString());
    interfaceComboBox->setCurrentText(settings.value("interface", "can0").toString());
    modeComboBox->setCurrentText(settings.value("mode", "Normal").toString());
}

void ConfigDialog::saveSettings() const {
    QSettings settings("YourCompany", "YourApp");
    settings.setValue("baudRate", baudRateComboBox->currentText());
    settings.setValue("interface", interfaceComboBox->currentText());
    settings.setValue("mode", modeComboBox->currentText());
}
