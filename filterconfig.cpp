#include "FilterConfig.h"
#include "LoadCell.h"
#include <QDebug>
#include <QTimer>
#include <QEventLoop>

FilterConfig::FilterConfig(QWidget *parent, LoadCell &loadCell)
    : QDialog(parent), loadCell(loadCell), canId(loadCell.getCanId()) {
    initUI();
}

void FilterConfig::initUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);

    adcSamplingSpeedInput = new QLineEdit(this);
    firFilteringInput = new QCheckBox("Enable FIR Filtering", this);
    filteringLevelInput = new QSpinBox(this);
    filteringLevelInput->setRange(1, 5);
    filteringBandwidthInput = new QLineEdit(this);
    autoZeroTrackingTimeInput = new QLineEdit(this);
    autoZeroTrackingBandwidthInput = new QLineEdit(this);
    stabilityDeterminationTimeInput = new QLineEdit(this);
    stabilityBandwidthInput = new QLineEdit(this);

    submitButton = new QPushButton("Submit", this);
    connect(submitButton, &QPushButton::clicked, this, &FilterConfig::onSubmit);

    layout->addWidget(new QLabel("ADC Sampling Speed (Hz):", this));
    layout->addWidget(adcSamplingSpeedInput);
    layout->addWidget(firFilteringInput);
    layout->addWidget(new QLabel("Filtering Level:", this));
    layout->addWidget(filteringLevelInput);
    layout->addWidget(new QLabel("Filtering Bandwidth (Hz):", this));
    layout->addWidget(filteringBandwidthInput);
    layout->addWidget(new QLabel("Auto Zero Tracking Time (ms):", this));
    layout->addWidget(autoZeroTrackingTimeInput);
    layout->addWidget(new QLabel("Auto Zero Tracking Bandwidth (Hz):", this));
    layout->addWidget(autoZeroTrackingBandwidthInput);
    layout->addWidget(new QLabel("Stability Determination Time (ms):", this));
    layout->addWidget(stabilityDeterminationTimeInput);
    layout->addWidget(new QLabel("Stability Bandwidth (Hz):", this));
    layout->addWidget(stabilityBandwidthInput);
    layout->addWidget(submitButton);
}

void FilterConfig::onSubmit() {
    uint32_t adcSamplingSpeed = adcSamplingSpeedInput->text().toUInt();
    bool firFiltering = firFilteringInput->isChecked();
    uint8_t filteringLevel = filteringLevelInput->value();
    uint32_t filteringBandwidth = filteringBandwidthInput->text().toUInt();
    uint16_t autoZeroTrackingTime = autoZeroTrackingTimeInput->text().toUInt();
    uint32_t autoZeroTrackingBandwidth = autoZeroTrackingBandwidthInput->text().toUInt();
    uint32_t stabilityDeterminationTime = stabilityDeterminationTimeInput->text().toUInt();
    uint32_t stabilityBandwidth = stabilityBandwidthInput->text().toUInt();

    loadCell.disconnectUpdateWeight();

    // Create a QTimer to handle the delay
    QTimer timer;
    timer.setSingleShot(true);
    connect(&timer, &QTimer::timeout, this, [this, adcSamplingSpeed, firFiltering, filteringLevel, filteringBandwidth, autoZeroTrackingTime, autoZeroTrackingBandwidth, stabilityDeterminationTime, stabilityBandwidth]() {
        loadCell.setADCSamplingSpeed(adcSamplingSpeed);
        loadCell.setFIRFiltering(firFiltering);
        loadCell.setFilteringLevel(filteringLevel);
        loadCell.setFilteringBandwidth(filteringBandwidth);
        loadCell.setAutoZeroTrackingTime(autoZeroTrackingTime);
        loadCell.setAutoZeroTrackingBandwidth(autoZeroTrackingBandwidth);
        loadCell.setStabilityDeterminationTime(stabilityDeterminationTime);
        loadCell.setStabilityBandwidth(stabilityBandwidth);
        loadCell.connectUpdateWeight();
        this->accept();  // Close the dialog after submitting
    });

    timer.start(100);  // 10 ms delay

    // Enter event loop to wait for the timer
    QEventLoop loop;
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    loop.exec();
}
