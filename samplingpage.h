#ifndef SAMPLINGPAGE_H
#define SAMPLINGPAGE_H

#include <QWidget>
#include <QTimer>
#include <QFile>
#include "mainwindow.h"

namespace Ui {
class SamplingPage;
}

class LoadCell; // Forward declaration

class SamplingPage : public QWidget
{
    Q_OBJECT

public:
    explicit SamplingPage(QWidget *parent = nullptr, MainWindow *mainWindow = nullptr);
    ~SamplingPage();

private slots:
    void startSampling();
    void sampleData();

private:
    Ui::SamplingPage *ui;
    MainWindow *mainWindow;
    QTimer samplingTimer;
    int remainingSamples;
    int samplingFrequency;
    QString timestamp;
    QString filename;
    QFile *file; // Declare file pointer
};

#endif // SAMPLINGPAGE_H
