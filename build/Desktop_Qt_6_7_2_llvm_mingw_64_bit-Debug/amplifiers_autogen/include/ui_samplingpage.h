/********************************************************************************
** Form generated from reading UI file 'samplingpage.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMPLINGPAGE_H
#define UI_SAMPLINGPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamplingPage
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *samplingGroupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *intervalLayout;
    QLabel *intervalLabel;
    QSpinBox *intervalSpinBox;
    QHBoxLayout *durationLayout;
    QLabel *durationLabel;
    QSpinBox *durationSpinBox;
    QPushButton *startButton;

    void setupUi(QWidget *SamplingPage)
    {
        if (SamplingPage->objectName().isEmpty())
            SamplingPage->setObjectName("SamplingPage");
        SamplingPage->resize(400, 200);
        verticalLayout = new QVBoxLayout(SamplingPage);
        verticalLayout->setObjectName("verticalLayout");
        samplingGroupBox = new QGroupBox(SamplingPage);
        samplingGroupBox->setObjectName("samplingGroupBox");
        verticalLayout_2 = new QVBoxLayout(samplingGroupBox);
        verticalLayout_2->setObjectName("verticalLayout_2");
        intervalLayout = new QHBoxLayout();
        intervalLayout->setObjectName("intervalLayout");
        intervalLabel = new QLabel(samplingGroupBox);
        intervalLabel->setObjectName("intervalLabel");

        intervalLayout->addWidget(intervalLabel);

        intervalSpinBox = new QSpinBox(samplingGroupBox);
        intervalSpinBox->setObjectName("intervalSpinBox");
        intervalSpinBox->setMinimum(1);
        intervalSpinBox->setMaximum(1000);
        intervalSpinBox->setValue(1);

        intervalLayout->addWidget(intervalSpinBox);


        verticalLayout_2->addLayout(intervalLayout);

        durationLayout = new QHBoxLayout();
        durationLayout->setObjectName("durationLayout");
        durationLabel = new QLabel(samplingGroupBox);
        durationLabel->setObjectName("durationLabel");

        durationLayout->addWidget(durationLabel);

        durationSpinBox = new QSpinBox(samplingGroupBox);
        durationSpinBox->setObjectName("durationSpinBox");
        durationSpinBox->setMinimum(1);
        durationSpinBox->setMaximum(3600);
        durationSpinBox->setValue(10);

        durationLayout->addWidget(durationSpinBox);


        verticalLayout_2->addLayout(durationLayout);


        verticalLayout->addWidget(samplingGroupBox);

        startButton = new QPushButton(SamplingPage);
        startButton->setObjectName("startButton");

        verticalLayout->addWidget(startButton);


        retranslateUi(SamplingPage);

        QMetaObject::connectSlotsByName(SamplingPage);
    } // setupUi

    void retranslateUi(QWidget *SamplingPage)
    {
        SamplingPage->setWindowTitle(QCoreApplication::translate("SamplingPage", "Sampling Page", nullptr));
        samplingGroupBox->setTitle(QCoreApplication::translate("SamplingPage", "Sampling Configuration", nullptr));
        intervalLabel->setText(QCoreApplication::translate("SamplingPage", "Sampling Interval (ms):", nullptr));
        durationLabel->setText(QCoreApplication::translate("SamplingPage", "Duration (s):", nullptr));
        startButton->setText(QCoreApplication::translate("SamplingPage", "Start Sampling", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SamplingPage: public Ui_SamplingPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMPLINGPAGE_H
