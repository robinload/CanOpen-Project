#ifndef FILTERCONFIG_H
#define FILTERCONFIG_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "LoadCell.h"

class FilterConfig : public QDialog {
    Q_OBJECT

public:
    explicit FilterConfig(QWidget *parent, LoadCell &loadCell);

private slots:
    void onSubmit();

private:
    void initUI();

    LoadCell &loadCell;
    uint32_t canId;

    QLineEdit *adcSamplingSpeedInput;
    QCheckBox *firFilteringInput;
    QSpinBox *filteringLevelInput;
    QLineEdit *filteringBandwidthInput;
    QLineEdit *autoZeroTrackingTimeInput;
    QLineEdit *autoZeroTrackingBandwidthInput;
    QLineEdit *stabilityDeterminationTimeInput;
    QLineEdit *stabilityBandwidthInput;

    QPushButton *submitButton;
};

#endif // FILTERCONFIG_H
