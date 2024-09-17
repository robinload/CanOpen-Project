/********************************************************************************
** Form generated from reading UI file 'advancedcommanddialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADVANCEDCOMMANDDIALOG_H
#define UI_ADVANCEDCOMMANDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_advancedCommandDialog
{
public:
    QStackedWidget *stackedWidget;
    QWidget *page1;
    QVBoxLayout *verticalLayout;
    QLabel *passwordLabel;
    QLineEdit *passwordLineEdit;
    QPushButton *nextButton;
    QWidget *page2;
    QVBoxLayout *verticalLayout_2;
    QLabel *idLabel;
    QLineEdit *idLineEdit;
    QLabel *dataLabel;
    QLineEdit *dataByte1LineEdit;
    QLineEdit *dataByte2LineEdit;
    QLineEdit *dataByte3LineEdit;
    QLineEdit *dataByte4LineEdit;
    QLineEdit *dataByte5LineEdit;
    QPushButton *submitButton;

    void setupUi(QDialog *advancedCommandDialog)
    {
        if (advancedCommandDialog->objectName().isEmpty())
            advancedCommandDialog->setObjectName("advancedCommandDialog");
        advancedCommandDialog->resize(400, 300);
        stackedWidget = new QStackedWidget(advancedCommandDialog);
        stackedWidget->setObjectName("stackedWidget");
        page1 = new QWidget();
        page1->setObjectName("page1");
        verticalLayout = new QVBoxLayout(page1);
        verticalLayout->setObjectName("verticalLayout");
        passwordLabel = new QLabel(page1);
        passwordLabel->setObjectName("passwordLabel");

        verticalLayout->addWidget(passwordLabel);

        passwordLineEdit = new QLineEdit(page1);
        passwordLineEdit->setObjectName("passwordLineEdit");

        verticalLayout->addWidget(passwordLineEdit);

        nextButton = new QPushButton(page1);
        nextButton->setObjectName("nextButton");

        verticalLayout->addWidget(nextButton);

        stackedWidget->addWidget(page1);
        page2 = new QWidget();
        page2->setObjectName("page2");
        verticalLayout_2 = new QVBoxLayout(page2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        idLabel = new QLabel(page2);
        idLabel->setObjectName("idLabel");

        verticalLayout_2->addWidget(idLabel);

        idLineEdit = new QLineEdit(page2);
        idLineEdit->setObjectName("idLineEdit");

        verticalLayout_2->addWidget(idLineEdit);

        dataLabel = new QLabel(page2);
        dataLabel->setObjectName("dataLabel");

        verticalLayout_2->addWidget(dataLabel);

        dataByte1LineEdit = new QLineEdit(page2);
        dataByte1LineEdit->setObjectName("dataByte1LineEdit");

        verticalLayout_2->addWidget(dataByte1LineEdit);

        dataByte2LineEdit = new QLineEdit(page2);
        dataByte2LineEdit->setObjectName("dataByte2LineEdit");

        verticalLayout_2->addWidget(dataByte2LineEdit);

        dataByte3LineEdit = new QLineEdit(page2);
        dataByte3LineEdit->setObjectName("dataByte3LineEdit");

        verticalLayout_2->addWidget(dataByte3LineEdit);

        dataByte4LineEdit = new QLineEdit(page2);
        dataByte4LineEdit->setObjectName("dataByte4LineEdit");

        verticalLayout_2->addWidget(dataByte4LineEdit);

        dataByte5LineEdit = new QLineEdit(page2);
        dataByte5LineEdit->setObjectName("dataByte5LineEdit");

        verticalLayout_2->addWidget(dataByte5LineEdit);

        submitButton = new QPushButton(page2);
        submitButton->setObjectName("submitButton");

        verticalLayout_2->addWidget(submitButton);

        stackedWidget->addWidget(page2);

        retranslateUi(advancedCommandDialog);

        QMetaObject::connectSlotsByName(advancedCommandDialog);
    } // setupUi

    void retranslateUi(QDialog *advancedCommandDialog)
    {
        advancedCommandDialog->setWindowTitle(QCoreApplication::translate("advancedCommandDialog", "Advanced Command Dialog", nullptr));

        // Add title labels to your pages if needed
        QLabel *page1Title = new QLabel(page1);
        page1Title->setText(QCoreApplication::translate("advancedCommandDialog", "Page 1", nullptr));
        verticalLayout->insertWidget(0, page1Title);  // Insert at the top of page1

        QLabel *page2Title = new QLabel(page2);
        page2Title->setText(QCoreApplication::translate("advancedCommandDialog", "Page 2", nullptr));
        verticalLayout_2->insertWidget(0, page2Title);  // Insert at the top of page2

        passwordLabel->setText(QCoreApplication::translate("advancedCommandDialog", "Password:", nullptr));
        nextButton->setText(QCoreApplication::translate("advancedCommandDialog", "Next", nullptr));
        idLabel->setText(QCoreApplication::translate("advancedCommandDialog", "ID:", nullptr));
        dataLabel->setText(QCoreApplication::translate("advancedCommandDialog", "Data Bytes (2 digits each):", nullptr));
        submitButton->setText(QCoreApplication::translate("advancedCommandDialog", "Submit", nullptr));
    }

};

namespace Ui {
    class advancedCommandDialog: public Ui_advancedCommandDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADVANCEDCOMMANDDIALOG_H
