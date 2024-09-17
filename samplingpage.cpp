#include "samplingpage.h"
#include "ui_samplingpage.h"
#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

SamplingPage::SamplingPage(QWidget *parent, MainWindow *mainWindow) :
    QWidget(parent),
    ui(new Ui::SamplingPage),
    mainWindow(mainWindow),
    remainingSamples(0),
    samplingFrequency(0),
    file(nullptr) // Initialize file pointer
{
    ui->setupUi(this);
    connect(ui->startButton, &QPushButton::clicked, this, &SamplingPage::startSampling);
}

SamplingPage::~SamplingPage()
{
    delete ui;
    if (file && file->isOpen()) {
        file->close(); // Close the file if it's open
    }
    delete file; // Delete the QFile pointer
}

void SamplingPage::startSampling()
{
    qDebug() << "startSampling called";

    int interval = ui->intervalSpinBox->value();
    int duration = ui->durationSpinBox->value();

    qDebug() << "Interval:" << interval;
    qDebug() << "Duration:" << duration;

    samplingFrequency = 1000 / interval; // Convert interval to milliseconds
    remainingSamples = duration;

    qDebug() << "Sampling frequency (ms):" << samplingFrequency;
    qDebug() << "Remaining samples:" << remainingSamples;

    // Generate the filename only once
    timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    filename = QString("loadcell_data_%1.csv").arg(timestamp);

    file = new QFile(filename);
    if (!file->open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing";
        return;
    } else {
        qDebug() << "File created successfully:" << filename;
    }

    QTextStream out(file);
    out << "Timestamp";
    for (int i = 0; i < mainWindow->getLoadCells().size(); ++i) {
        out << ",Load Cell " << i + 1;
    }
    out << "\n";

    samplingTimer.setInterval(samplingFrequency);
    connect(&samplingTimer, &QTimer::timeout, this, &SamplingPage::sampleData);
    samplingTimer.start();

    qDebug() << "Sampling timer started with interval:" << samplingFrequency << "ms";
}

void SamplingPage::sampleData()
{
    qDebug() << "sampleData called";

    if (remainingSamples <= 0) {
        qDebug() << "No remaining samples. Stopping timer.";
        samplingTimer.stop();
        if (file && file->isOpen()) {
            file->close(); // Close the file if it's open
        }
        return;
    }

    if (file->isOpen()) {
        QTextStream out(file);
        QString timeStampStr = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        qDebug() << "Writing data with timestamp:" << timeStampStr;
        out << timeStampStr;

        for (LoadCell *loadCell : mainWindow->getLoadCells()) {
            out << "," << loadCell->currentWeight();
            qDebug() << "Load Cell Weight:" << loadCell->currentWeight();
        }
        out << "\n";

        remainingSamples--;
        qDebug() << "Remaining samples after this iteration:" << remainingSamples;
    } else {
        qDebug() << "File is not open for writing";
    }
}
