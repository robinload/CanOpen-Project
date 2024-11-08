#ifndef SAMPLINGPAGE_H
#define SAMPLINGPAGE_H

#include <QWidget>
#include <QTimer>
#include <QFile>
#include <QVector>
#include <QString>
#include <QMessageBox>

namespace Ui {
class SamplingPage;
}

class LoadCell; // Forward declaration
class MainWindow; // Forward declaration

class SamplingPage : public QWidget
{
    Q_OBJECT

public:
    explicit SamplingPage(QWidget *parent = nullptr, MainWindow *mainWindow = nullptr);
    ~SamplingPage();

private slots:
    void startSampling();
    void sampleData();
    void showCountdownMessage(int durationInSeconds); // Function to show countdown

private:
    QVector<QString> collectedData;  // A vector to store collected data lines
    Ui::SamplingPage *ui;
    MainWindow *mainWindow;
    QTimer samplingTimer;
    int remainingSamples;
    int samplingFrequency;
    QString timestamp;
    QString filename;
    QFile *file; // Declare file pointer
    QMessageBox *samplingMsgBox; // Pointer to the message box
};

#endif // SAMPLINGPAGE_H
