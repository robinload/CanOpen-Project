/********************************************************************************
** Form generated from reading UI file 'commanddialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMMANDDIALOG_H
#define UI_COMMANDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CommandDialog
{
public:
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *passwordPage;
    QVBoxLayout *verticalLayout_2;
    QLabel *labelPassword;
    QLineEdit *passwordLineEdit;
    QPushButton *submitButton;
    QWidget *commandPage;
    QVBoxLayout *verticalLayout_3;
    QLabel *labelCommand;
    QHBoxLayout *commandLayout;
    QLineEdit *byte1LineEdit;
    QLineEdit *byte2LineEdit;
    QLineEdit *byte3LineEdit;
    QLineEdit *byte4LineEdit;
    QLineEdit *byte5LineEdit;
    QPushButton *sendCommandButton;

    void setupUi(QDialog *CommandDialog)
    {
        if (CommandDialog->objectName().isEmpty())
            CommandDialog->setObjectName("CommandDialog");
        CommandDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(CommandDialog);
        verticalLayout->setObjectName("verticalLayout");
        stackedWidget = new QStackedWidget(CommandDialog);
        stackedWidget->setObjectName("stackedWidget");
        passwordPage = new QWidget();
        passwordPage->setObjectName("passwordPage");
        verticalLayout_2 = new QVBoxLayout(passwordPage);
        verticalLayout_2->setObjectName("verticalLayout_2");
        labelPassword = new QLabel(passwordPage);
        labelPassword->setObjectName("labelPassword");

        verticalLayout_2->addWidget(labelPassword);

        passwordLineEdit = new QLineEdit(passwordPage);
        passwordLineEdit->setObjectName("passwordLineEdit");
        passwordLineEdit->setEchoMode(QLineEdit::Password);

        verticalLayout_2->addWidget(passwordLineEdit);

        submitButton = new QPushButton(passwordPage);
        submitButton->setObjectName("submitButton");

        verticalLayout_2->addWidget(submitButton);

        stackedWidget->addWidget(passwordPage);
        commandPage = new QWidget();
        commandPage->setObjectName("commandPage");
        verticalLayout_3 = new QVBoxLayout(commandPage);
        verticalLayout_3->setObjectName("verticalLayout_3");
        labelCommand = new QLabel(commandPage);
        labelCommand->setObjectName("labelCommand");

        verticalLayout_3->addWidget(labelCommand);

        commandLayout = new QHBoxLayout();
        commandLayout->setObjectName("commandLayout");
        byte1LineEdit = new QLineEdit(commandPage);
        byte1LineEdit->setObjectName("byte1LineEdit");
        byte1LineEdit->setMaxLength(2);
        byte1LineEdit->setAlignment(Qt::AlignCenter);

        commandLayout->addWidget(byte1LineEdit);

        byte2LineEdit = new QLineEdit(commandPage);
        byte2LineEdit->setObjectName("byte2LineEdit");
        byte2LineEdit->setMaxLength(2);
        byte2LineEdit->setAlignment(Qt::AlignCenter);

        commandLayout->addWidget(byte2LineEdit);

        byte3LineEdit = new QLineEdit(commandPage);
        byte3LineEdit->setObjectName("byte3LineEdit");
        byte3LineEdit->setMaxLength(2);
        byte3LineEdit->setAlignment(Qt::AlignCenter);

        commandLayout->addWidget(byte3LineEdit);

        byte4LineEdit = new QLineEdit(commandPage);
        byte4LineEdit->setObjectName("byte4LineEdit");
        byte4LineEdit->setMaxLength(2);
        byte4LineEdit->setAlignment(Qt::AlignCenter);

        commandLayout->addWidget(byte4LineEdit);

        byte5LineEdit = new QLineEdit(commandPage);
        byte5LineEdit->setObjectName("byte5LineEdit");
        byte5LineEdit->setMaxLength(2);
        byte5LineEdit->setAlignment(Qt::AlignCenter);

        commandLayout->addWidget(byte5LineEdit);


        verticalLayout_3->addLayout(commandLayout);

        sendCommandButton = new QPushButton(commandPage);
        sendCommandButton->setObjectName("sendCommandButton");

        verticalLayout_3->addWidget(sendCommandButton);

        stackedWidget->addWidget(commandPage);

        verticalLayout->addWidget(stackedWidget);


        retranslateUi(CommandDialog);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(CommandDialog);
    } // setupUi

    void retranslateUi(QDialog *CommandDialog)
    {
        CommandDialog->setWindowTitle(QCoreApplication::translate("CommandDialog", "CANBUS Command", nullptr));
        passwordPage->setProperty("title", QVariant(QCoreApplication::translate("CommandDialog", "Password", nullptr)));
        labelPassword->setText(QCoreApplication::translate("CommandDialog", "Enter Password", nullptr));
        submitButton->setText(QCoreApplication::translate("CommandDialog", "Submit", nullptr));
        commandPage->setProperty("title", QVariant(QCoreApplication::translate("CommandDialog", "Send CANBUS Command", nullptr)));
        labelCommand->setText(QCoreApplication::translate("CommandDialog", "Enter CANBUS Command (5 Bytes)", nullptr));
        byte1LineEdit->setInputMask(QCoreApplication::translate("CommandDialog", "HH", nullptr));
        byte2LineEdit->setInputMask(QCoreApplication::translate("CommandDialog", "HH", nullptr));
        byte3LineEdit->setInputMask(QCoreApplication::translate("CommandDialog", "HH", nullptr));
        byte4LineEdit->setInputMask(QCoreApplication::translate("CommandDialog", "HH", nullptr));
        byte5LineEdit->setInputMask(QCoreApplication::translate("CommandDialog", "HH", nullptr));
        sendCommandButton->setText(QCoreApplication::translate("CommandDialog", "Send Command", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CommandDialog: public Ui_CommandDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMMANDDIALOG_H
