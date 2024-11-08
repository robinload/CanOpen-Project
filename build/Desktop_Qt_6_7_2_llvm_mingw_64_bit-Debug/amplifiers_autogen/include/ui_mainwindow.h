/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QCustomPlot *mainPlot;
    QPushButton *addButton;
    QPushButton *removeButton;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *poundCheckbox;
    QCheckBox *gramCheckbox;
    QCheckBox *kilogramCheckbox;
    QLabel *label_2;
    QWidget *tab_2;
    QCheckBox *autoBox;
    QPushButton *CSVButton;
    QCheckBox *manualCheckbox;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *maxLabel;
    QSpinBox *maxBox;
    QHBoxLayout *horizontalLayout;
    QLabel *minLabel;
    QSpinBox *minBox;
    QPushButton *startButton;
    QPushButton *stopButton;
    QWidget *verticalLayoutWidget_5;
    QVBoxLayout *verticalLayout_5;
    QPushButton *saveCalibrationButton;
    QPushButton *loadCalibrationButton;
    QPushButton *advancedCommandButton;
    QPushButton *pushButton_3;
    QPushButton *filterConfigButton;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QLabel *statusLabel;
    QSpacerItem *horizontalSpacer;
    QWidget *verticalLayoutWidget_4;
    QVBoxLayout *verticalLayout_6;
    QLabel *label;
    QVBoxLayout *verticalLayout;
    QMenuBar *menubar;
    QMenu *menuHi;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1347, 752);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        mainPlot = new QCustomPlot(centralwidget);
        mainPlot->setObjectName("mainPlot");
        mainPlot->setGeometry(QRect(250, 80, 851, 611));
        addButton = new QPushButton(centralwidget);
        addButton->setObjectName("addButton");
        addButton->setGeometry(QRect(20, 670, 101, 31));
        removeButton = new QPushButton(centralwidget);
        removeButton->setObjectName("removeButton");
        removeButton->setGeometry(QRect(150, 670, 101, 31));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(1120, 50, 181, 281));
        tab = new QWidget();
        tab->setObjectName("tab");
        verticalLayoutWidget_2 = new QWidget(tab);
        verticalLayoutWidget_2->setObjectName("verticalLayoutWidget_2");
        verticalLayoutWidget_2->setGeometry(QRect(10, 40, 151, 78));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        poundCheckbox = new QCheckBox(verticalLayoutWidget_2);
        poundCheckbox->setObjectName("poundCheckbox");

        verticalLayout_2->addWidget(poundCheckbox);

        gramCheckbox = new QCheckBox(verticalLayoutWidget_2);
        gramCheckbox->setObjectName("gramCheckbox");
        gramCheckbox->setChecked(true);

        verticalLayout_2->addWidget(gramCheckbox);

        kilogramCheckbox = new QCheckBox(verticalLayoutWidget_2);
        kilogramCheckbox->setObjectName("kilogramCheckbox");

        verticalLayout_2->addWidget(kilogramCheckbox);

        label_2 = new QLabel(tab);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(10, 10, 141, 20));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        autoBox = new QCheckBox(tab_2);
        autoBox->setObjectName("autoBox");
        autoBox->setEnabled(true);
        autoBox->setGeometry(QRect(10, 10, 161, 21));
        autoBox->setChecked(true);
        CSVButton = new QPushButton(tab_2);
        CSVButton->setObjectName("CSVButton");
        CSVButton->setGeometry(QRect(10, 190, 161, 31));
        manualCheckbox = new QCheckBox(tab_2);
        manualCheckbox->setObjectName("manualCheckbox");
        manualCheckbox->setGeometry(QRect(10, 40, 101, 21));
        verticalLayoutWidget_3 = new QWidget(tab_2);
        verticalLayoutWidget_3->setObjectName("verticalLayoutWidget_3");
        verticalLayoutWidget_3->setGeometry(QRect(10, 70, 160, 111));
        verticalLayout_3 = new QVBoxLayout(verticalLayoutWidget_3);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        maxLabel = new QLabel(verticalLayoutWidget_3);
        maxLabel->setObjectName("maxLabel");

        horizontalLayout_2->addWidget(maxLabel);

        maxBox = new QSpinBox(verticalLayoutWidget_3);
        maxBox->setObjectName("maxBox");
        maxBox->setMinimum(-999999999);
        maxBox->setMaximum(999999999);

        horizontalLayout_2->addWidget(maxBox);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        minLabel = new QLabel(verticalLayoutWidget_3);
        minLabel->setObjectName("minLabel");

        horizontalLayout->addWidget(minLabel);

        minBox = new QSpinBox(verticalLayoutWidget_3);
        minBox->setObjectName("minBox");
        minBox->setMinimum(-999999999);
        minBox->setMaximum(999999999);

        horizontalLayout->addWidget(minBox);


        verticalLayout_3->addLayout(horizontalLayout);

        tabWidget->addTab(tab_2, QString());
        startButton = new QPushButton(centralwidget);
        startButton->setObjectName("startButton");
        startButton->setGeometry(QRect(900, 20, 80, 22));
        stopButton = new QPushButton(centralwidget);
        stopButton->setObjectName("stopButton");
        stopButton->setGeometry(QRect(1000, 20, 80, 22));
        verticalLayoutWidget_5 = new QWidget(centralwidget);
        verticalLayoutWidget_5->setObjectName("verticalLayoutWidget_5");
        verticalLayoutWidget_5->setGeometry(QRect(1120, 360, 171, 331));
        verticalLayout_5 = new QVBoxLayout(verticalLayoutWidget_5);
        verticalLayout_5->setObjectName("verticalLayout_5");
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        saveCalibrationButton = new QPushButton(verticalLayoutWidget_5);
        saveCalibrationButton->setObjectName("saveCalibrationButton");

        verticalLayout_5->addWidget(saveCalibrationButton);

        loadCalibrationButton = new QPushButton(verticalLayoutWidget_5);
        loadCalibrationButton->setObjectName("loadCalibrationButton");

        verticalLayout_5->addWidget(loadCalibrationButton);

        advancedCommandButton = new QPushButton(verticalLayoutWidget_5);
        advancedCommandButton->setObjectName("advancedCommandButton");

        verticalLayout_5->addWidget(advancedCommandButton);

        pushButton_3 = new QPushButton(verticalLayoutWidget_5);
        pushButton_3->setObjectName("pushButton_3");

        verticalLayout_5->addWidget(pushButton_3);

        filterConfigButton = new QPushButton(verticalLayoutWidget_5);
        filterConfigButton->setObjectName("filterConfigButton");

        verticalLayout_5->addWidget(filterConfigButton);

        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(250, 0, 641, 61));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        connectButton = new QPushButton(horizontalLayoutWidget);
        connectButton->setObjectName("connectButton");

        horizontalLayout_3->addWidget(connectButton);

        disconnectButton = new QPushButton(horizontalLayoutWidget);
        disconnectButton->setObjectName("disconnectButton");

        horizontalLayout_3->addWidget(disconnectButton);

        statusLabel = new QLabel(horizontalLayoutWidget);
        statusLabel->setObjectName("statusLabel");

        horizontalLayout_3->addWidget(statusLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        verticalLayoutWidget_4 = new QWidget(centralwidget);
        verticalLayoutWidget_4->setObjectName("verticalLayoutWidget_4");
        verticalLayoutWidget_4->setGeometry(QRect(20, 80, 221, 571));
        verticalLayout_6 = new QVBoxLayout(verticalLayoutWidget_4);
        verticalLayout_6->setObjectName("verticalLayout_6");
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(verticalLayoutWidget_4);
        label->setObjectName("label");

        verticalLayout_6->addWidget(label);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");

        verticalLayout_6->addLayout(verticalLayout);

        verticalLayout_6->setStretch(1, 1);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1347, 19));
        menuHi = new QMenu(menubar);
        menuHi->setObjectName("menuHi");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuHi->menuAction());

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        addButton->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        removeButton->setText(QCoreApplication::translate("MainWindow", "Remove", nullptr));
        poundCheckbox->setText(QCoreApplication::translate("MainWindow", "Pound", nullptr));
        gramCheckbox->setText(QCoreApplication::translate("MainWindow", "Gram", nullptr));
        kilogramCheckbox->setText(QCoreApplication::translate("MainWindow", "Kilogram", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Weight Unit:", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Tab 1", nullptr));
        autoBox->setText(QCoreApplication::translate("MainWindow", "Auto Scaling", nullptr));
        CSVButton->setText(QCoreApplication::translate("MainWindow", "CSV Output", nullptr));
        manualCheckbox->setText(QCoreApplication::translate("MainWindow", "Manual", nullptr));
        maxLabel->setText(QCoreApplication::translate("MainWindow", "MAX:", nullptr));
        minLabel->setText(QCoreApplication::translate("MainWindow", "MIN:", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Tab 2", nullptr));
        startButton->setText(QCoreApplication::translate("MainWindow", "START", nullptr));
        stopButton->setText(QCoreApplication::translate("MainWindow", "STOP", nullptr));
        saveCalibrationButton->setText(QCoreApplication::translate("MainWindow", "Save Calibration", nullptr));
        loadCalibrationButton->setText(QCoreApplication::translate("MainWindow", "Load Calibration ", nullptr));
        advancedCommandButton->setText(QCoreApplication::translate("MainWindow", "Advanced Send", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "Zero All", nullptr));
        filterConfigButton->setText(QCoreApplication::translate("MainWindow", "Filter Config", nullptr));
        connectButton->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        disconnectButton->setText(QCoreApplication::translate("MainWindow", "Disconnect", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "Status: Disconnected", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Connected Devices:", nullptr));
        menuHi->setTitle(QCoreApplication::translate("MainWindow", "ANYLOAD", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
