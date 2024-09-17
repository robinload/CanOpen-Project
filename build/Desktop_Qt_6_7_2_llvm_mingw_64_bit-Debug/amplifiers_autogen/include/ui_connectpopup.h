/********************************************************************************
** Form generated from reading UI file 'connectpopup.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTPOPUP_H
#define UI_CONNECTPOPUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_connectPopup
{
public:
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *currentCountNumberLabel;
    QLabel *ADCValueLabel;
    QLabel *currentWeightTextLabel;
    QDoubleSpinBox *weightBox;
    QComboBox *unitBox;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *setPointButton;
    QPushButton *nextPointButton;
    QPushButton *calibrateButton;
    QSpinBox *CANIdbox;
    QLabel *label;
    QPushButton *connectButton;

    void setupUi(QWidget *connectPopup)
    {
        if (connectPopup->objectName().isEmpty())
            connectPopup->setObjectName("connectPopup");
        connectPopup->resize(643, 467);
        horizontalLayoutWidget_2 = new QWidget(connectPopup);
        horizontalLayoutWidget_2->setObjectName("horizontalLayoutWidget_2");
        horizontalLayoutWidget_2->setGeometry(QRect(10, 160, 621, 131));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        currentCountNumberLabel = new QLabel(horizontalLayoutWidget_2);
        currentCountNumberLabel->setObjectName("currentCountNumberLabel");

        horizontalLayout_2->addWidget(currentCountNumberLabel);

        ADCValueLabel = new QLabel(horizontalLayoutWidget_2);
        ADCValueLabel->setObjectName("ADCValueLabel");

        horizontalLayout_2->addWidget(ADCValueLabel);

        currentWeightTextLabel = new QLabel(horizontalLayoutWidget_2);
        currentWeightTextLabel->setObjectName("currentWeightTextLabel");

        horizontalLayout_2->addWidget(currentWeightTextLabel);

        weightBox = new QDoubleSpinBox(horizontalLayoutWidget_2);
        weightBox->setObjectName("weightBox");
        weightBox->setMaximum(9999999999.989999771118164);

        horizontalLayout_2->addWidget(weightBox);

        unitBox = new QComboBox(horizontalLayoutWidget_2);
        unitBox->addItem(QString());
        unitBox->addItem(QString());
        unitBox->addItem(QString());
        unitBox->setObjectName("unitBox");

        horizontalLayout_2->addWidget(unitBox);

        horizontalLayoutWidget_3 = new QWidget(connectPopup);
        horizontalLayoutWidget_3->setObjectName("horizontalLayoutWidget_3");
        horizontalLayoutWidget_3->setGeometry(QRect(10, 300, 621, 51));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        setPointButton = new QPushButton(horizontalLayoutWidget_3);
        setPointButton->setObjectName("setPointButton");

        horizontalLayout_3->addWidget(setPointButton);

        nextPointButton = new QPushButton(horizontalLayoutWidget_3);
        nextPointButton->setObjectName("nextPointButton");

        horizontalLayout_3->addWidget(nextPointButton);

        calibrateButton = new QPushButton(connectPopup);
        calibrateButton->setObjectName("calibrateButton");
        calibrateButton->setGeometry(QRect(220, 430, 203, 24));
        CANIdbox = new QSpinBox(connectPopup);
        CANIdbox->setObjectName("CANIdbox");
        CANIdbox->setGeometry(QRect(110, 50, 181, 41));
        CANIdbox->setMinimum(1);
        CANIdbox->setMaximum(127);
        label = new QLabel(connectPopup);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 60, 91, 21));
        connectButton = new QPushButton(connectPopup);
        connectButton->setObjectName("connectButton");
        connectButton->setGeometry(QRect(370, 50, 191, 31));

        retranslateUi(connectPopup);

        QMetaObject::connectSlotsByName(connectPopup);
    } // setupUi

    void retranslateUi(QWidget *connectPopup)
    {
        connectPopup->setWindowTitle(QCoreApplication::translate("connectPopup", "Form", nullptr));
        currentCountNumberLabel->setText(QCoreApplication::translate("connectPopup", "Current Points: 0", nullptr));
        ADCValueLabel->setText(QCoreApplication::translate("connectPopup", "AD Value:", nullptr));
        currentWeightTextLabel->setText(QCoreApplication::translate("connectPopup", "Current Weight", nullptr));
        unitBox->setItemText(0, QCoreApplication::translate("connectPopup", "kg", nullptr));
        unitBox->setItemText(1, QCoreApplication::translate("connectPopup", "lb", nullptr));
        unitBox->setItemText(2, QCoreApplication::translate("connectPopup", "g", nullptr));

        setPointButton->setText(QCoreApplication::translate("connectPopup", "Set Point", nullptr));
        nextPointButton->setText(QCoreApplication::translate("connectPopup", "Next Point", nullptr));
        calibrateButton->setText(QCoreApplication::translate("connectPopup", "Calibrate", nullptr));
        label->setText(QCoreApplication::translate("connectPopup", "CAN ID", nullptr));
        connectButton->setText(QCoreApplication::translate("connectPopup", "Connect", nullptr));
    } // retranslateUi

};

namespace Ui {
    class connectPopup: public Ui_connectPopup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTPOPUP_H
