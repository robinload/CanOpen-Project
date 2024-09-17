/********************************************************************************
** Form generated from reading UI file 'advanceddialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADVANCEDDIALOG_H
#define UI_ADVANCEDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_advancedDialog
{
public:
    QStackedWidget *stackedWidget;
    QWidget *page;
    QLineEdit *passwordLineEdit;
    QPushButton *nextPageButton;
    QLabel *label;
    QWidget *page_2;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLineEdit *byte1Edit;
    QLineEdit *byte2Edit;
    QLineEdit *byte3Edit;
    QLineEdit *lineEdibyte4Editt_8;
    QLineEdit *byte5Edit;
    QLabel *label_3;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *canIDLineEdit;

    void setupUi(QDialog *advancedDialog)
    {
        if (advancedDialog->objectName().isEmpty())
            advancedDialog->setObjectName("advancedDialog");
        advancedDialog->resize(400, 300);
        stackedWidget = new QStackedWidget(advancedDialog);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(10, 20, 371, 271));
        page = new QWidget();
        page->setObjectName("page");
        passwordLineEdit = new QLineEdit(page);
        passwordLineEdit->setObjectName("passwordLineEdit");
        passwordLineEdit->setGeometry(QRect(0, 190, 381, 31));
        nextPageButton = new QPushButton(page);
        nextPageButton->setObjectName("nextPageButton");
        nextPageButton->setGeometry(QRect(140, 230, 91, 21));
        label = new QLabel(page);
        label->setObjectName("label");
        label->setGeometry(QRect(140, 110, 81, 41));
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        horizontalLayoutWidget = new QWidget(page_2);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(10, 170, 361, 80));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        byte1Edit = new QLineEdit(horizontalLayoutWidget);
        byte1Edit->setObjectName("byte1Edit");

        horizontalLayout->addWidget(byte1Edit);

        byte2Edit = new QLineEdit(horizontalLayoutWidget);
        byte2Edit->setObjectName("byte2Edit");

        horizontalLayout->addWidget(byte2Edit);

        byte3Edit = new QLineEdit(horizontalLayoutWidget);
        byte3Edit->setObjectName("byte3Edit");

        horizontalLayout->addWidget(byte3Edit);

        lineEdibyte4Editt_8 = new QLineEdit(horizontalLayoutWidget);
        lineEdibyte4Editt_8->setObjectName("lineEdibyte4Editt_8");

        horizontalLayout->addWidget(lineEdibyte4Editt_8);

        byte5Edit = new QLineEdit(horizontalLayoutWidget);
        byte5Edit->setObjectName("byte5Edit");

        horizontalLayout->addWidget(byte5Edit);

        label_3 = new QLabel(page_2);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(10, 140, 91, 16));
        horizontalLayoutWidget_2 = new QWidget(page_2);
        horizontalLayoutWidget_2->setObjectName("horizontalLayoutWidget_2");
        horizontalLayoutWidget_2->setGeometry(QRect(10, 70, 171, 41));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(horizontalLayoutWidget_2);
        label_2->setObjectName("label_2");

        horizontalLayout_2->addWidget(label_2);

        canIDLineEdit = new QLineEdit(horizontalLayoutWidget_2);
        canIDLineEdit->setObjectName("canIDLineEdit");

        horizontalLayout_2->addWidget(canIDLineEdit);

        stackedWidget->addWidget(page_2);

        retranslateUi(advancedDialog);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(advancedDialog);
    } // setupUi

    void retranslateUi(QDialog *advancedDialog)
    {
        advancedDialog->setWindowTitle(QCoreApplication::translate("advancedDialog", "Dialog", nullptr));
        nextPageButton->setText(QCoreApplication::translate("advancedDialog", "Next", nullptr));
        label->setText(QCoreApplication::translate("advancedDialog", "Enter Password", nullptr));
        label_3->setText(QCoreApplication::translate("advancedDialog", "Data:", nullptr));
        label_2->setText(QCoreApplication::translate("advancedDialog", "CAN ID", nullptr));
    } // retranslateUi

};

namespace Ui {
    class advancedDialog: public Ui_advancedDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADVANCEDDIALOG_H
