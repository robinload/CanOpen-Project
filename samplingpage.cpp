#include "samplingpage.h"
#include "ui_samplingpage.h"
#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>

SamplingPage::SamplingPage(QWidget *parent, MainWindow *mainWindow) :
    QWidget(parent),
    ui(new Ui::SamplingPage),
    mainWindow(mainWindow),
    remainingSamples(0),
    samplingFrequency(0),
    file(nullptr), // Initialize file pointer
    samplingMsgBox(nullptr) // Initialize message box pointer
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
    delete samplingMsgBox; // Delete the QMessageBox pointer
}

void SamplingPage::startSampling()
{
    qDebug() << "startSampling called";

    int interval = ui->intervalSpinBox->value();  // Interval is already in milliseconds
    int duration = ui->durationSpinBox->value();  // Duration is in seconds

    qDebug() << "Interval (ms):" << interval;
    qDebug() << "Duration (s):" << duration;

    samplingFrequency = interval;  // No need to convert, it's already in milliseconds
    remainingSamples = (duration * 1000) / interval;  // Calculate how many samples we need based on the interval and duration in seconds

    qDebug() << "Sampling frequency (ms):" << samplingFrequency;
    qDebug() << "Remaining samples (count):" << remainingSamples;

    // Show a countdown message before starting the sampling
    showCountdownMessage(5);  // Show a 5-second countdown

    QTimer::singleShot(5000, this, [=]() {  // Start sampling after countdown ends

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

        // Write the header to the file
        QTextStream out(file);
        out << "Timestamp";
        for (int i = 0; i < mainWindow->getLoadCells().size(); ++i) {
            out << ",Load Cell " << i + 1;
        }
        out << "\n";

        // Initialize data storage for sampling
        collectedData.clear();

        // Initialize the QMessageBox for sampling updates
        samplingMsgBox = new QMessageBox(this);
        samplingMsgBox->setWindowTitle("Sampling in Progress");
        samplingMsgBox->setText("Sampling has started...");
        samplingMsgBox->setStandardButtons(QMessageBox::NoButton); // Disable default buttons
        samplingMsgBox->show();

        samplingTimer.setInterval(samplingFrequency);
        connect(&samplingTimer, &QTimer::timeout, this, &SamplingPage::sampleData);
        samplingTimer.start();

        qDebug() << "Sampling timer started with interval:" << samplingFrequency << "ms";
    });
}

void SamplingPage::sampleData()
{
    qDebug() << "sampleData called";

    if (remainingSamples <= 0) {
        qDebug() << "No remaining samples. Stopping timer.";
        samplingTimer.stop();

        if (file && file->isOpen()) {
            // Write all buffered data to the file in one go
            QTextStream out(file);
            for (const QString &line : collectedData) {
                out << line << "\n";
            }
            file->close(); // Close the file after writing all data
            qDebug() << "All data written to file and file closed.";
        }

        if (samplingMsgBox) {
            samplingMsgBox->setText("Sampling completed. All data written to file.");
            QTimer::singleShot(2000, samplingMsgBox, &QMessageBox::accept); // Close the message box after 2 seconds
        }

        return;
    }

    // Buffer the data instead of writing it immediately
    QString timeStampStr = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString dataLine = timeStampStr;

    for (LoadCell *loadCell : mainWindow->getLoadCells()) {
        dataLine += "," + QString::number(loadCell->currentWeight());
        qDebug() << "Load Cell Weight:" << loadCell->currentWeight();
    }

    collectedData.append(dataLine);  // Store the data in memory
    qDebug() << "Buffered data:" << dataLine;

    remainingSamples--;
    qDebug() << "Remaining samples after this iteration:" << remainingSamples;

    // Update the QMessageBox with the latest data
    if (samplingMsgBox) {
        QString msgText = QString("Sampling in Progress...\n\nBuffered data: %1\nRemaining samples: %2")
                              .arg(dataLine)
                              .arg(remainingSamples);
        samplingMsgBox->setText(msgText);
    }
}

void SamplingPage::showCountdownMessage(int durationInSeconds)
{
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setWindowTitle("Countdown");
    msgBox->setText(QString("Sampling will start in %1 seconds...").arg(durationInSeconds));
    msgBox->setStandardButtons(QMessageBox::NoButton); // Disable default buttons
    msgBox->show();

    QTimer *countdownTimer = new QTimer(this);
    countdownTimer->setInterval(1000); // 1 second intervals

    connect(countdownTimer, &QTimer::timeout, this, [=]() mutable {
        --durationInSeconds;
        if (durationInSeconds > 0) {
            msgBox->setText(QString("Sampling will start in %1 seconds...").arg(durationInSeconds));
        } else {
            msgBox->setText("Sampling is starting now!");
            countdownTimer->stop();
            QTimer::singleShot(1000, msgBox, &QMessageBox::accept); // Close the message box after 1 second
        }
    });
    countdownTimer->start();
}
