/********************************************************************************
** Form generated from reading UI file 'calibrationPopup.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALIBRATIONPOPUP_H
#define UI_CALIBRATIONPOPUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayoutMain;
    QHBoxLayout *horizontalLayoutPort;
    QLabel *labelPort;
    QSpinBox *portBox;
    QPushButton *connectButton;
    QHBoxLayout *horizontalLayoutADC;
    QLabel *labelADC;
    QLabel *currentADCValue;
    QLabel *labelPointsCaptured;
    QLabel *pointsCapturedValue;
    QHBoxLayout *horizontalLayoutButtons;
    QPushButton *pushButtonSetPoint;
    QPushButton *pushButtonNextPoint;
    QPushButton *pushButtonCalibrate;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(857, 637);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayoutMain = new QVBoxLayout(centralwidget);
        verticalLayoutMain->setObjectName("verticalLayoutMain");
        horizontalLayoutPort = new QHBoxLayout();
        horizontalLayoutPort->setObjectName("horizontalLayoutPort");
        labelPort = new QLabel(centralwidget);
        labelPort->setObjectName("labelPort");

        horizontalLayoutPort->addWidget(labelPort);

        portBox = new QSpinBox(centralwidget);
        portBox->setObjectName("portBox");

        horizontalLayoutPort->addWidget(portBox);

        connectButton = new QPushButton(centralwidget);
        connectButton->setObjectName("connectButton");

        horizontalLayoutPort->addWidget(connectButton);


        verticalLayoutMain->addLayout(horizontalLayoutPort);

        horizontalLayoutADC = new QHBoxLayout();
        horizontalLayoutADC->setObjectName("horizontalLayoutADC");
        labelADC = new QLabel(centralwidget);
        labelADC->setObjectName("labelADC");

        horizontalLayoutADC->addWidget(labelADC);

        currentADCValue = new QLabel(centralwidget);
        currentADCValue->setObjectName("currentADCValue");

        horizontalLayoutADC->addWidget(currentADCValue);

        labelPointsCaptured = new QLabel(centralwidget);
        labelPointsCaptured->setObjectName("labelPointsCaptured");

        horizontalLayoutADC->addWidget(labelPointsCaptured);

        pointsCapturedValue = new QLabel(centralwidget);
        pointsCapturedValue->setObjectName("pointsCapturedValue");

        horizontalLayoutADC->addWidget(pointsCapturedValue);


        verticalLayoutMain->addLayout(horizontalLayoutADC);

        horizontalLayoutButtons = new QHBoxLayout();
        horizontalLayoutButtons->setObjectName("horizontalLayoutButtons");
        pushButtonSetPoint = new QPushButton(centralwidget);
        pushButtonSetPoint->setObjectName("pushButtonSetPoint");

        horizontalLayoutButtons->addWidget(pushButtonSetPoint);

        pushButtonNextPoint = new QPushButton(centralwidget);
        pushButtonNextPoint->setObjectName("pushButtonNextPoint");

        horizontalLayoutButtons->addWidget(pushButtonNextPoint);

        pushButtonCalibrate = new QPushButton(centralwidget);
        pushButtonCalibrate->setObjectName("pushButtonCalibrate");

        horizontalLayoutButtons->addWidget(pushButtonCalibrate);


        verticalLayoutMain->addLayout(horizontalLayoutButtons);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 857, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Calibration Window", nullptr));
        labelPort->setText(QCoreApplication::translate("MainWindow", "Select Port:", nullptr));
        connectButton->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        labelADC->setText(QCoreApplication::translate("MainWindow", "Current ADC Count:", nullptr));
        currentADCValue->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        labelPointsCaptured->setText(QCoreApplication::translate("MainWindow", "Points Captured:", nullptr));
        pointsCapturedValue->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        pushButtonSetPoint->setText(QCoreApplication::translate("MainWindow", "Set Point", nullptr));
        pushButtonNextPoint->setText(QCoreApplication::translate("MainWindow", "Next Point", nullptr));
        pushButtonCalibrate->setText(QCoreApplication::translate("MainWindow", "Calibrate", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALIBRATIONPOPUP_H
