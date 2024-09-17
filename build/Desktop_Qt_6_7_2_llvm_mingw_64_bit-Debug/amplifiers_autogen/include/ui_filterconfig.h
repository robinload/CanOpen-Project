/********************************************************************************
** Form generated from reading UI file 'filterconfig.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILTERCONFIG_H
#define UI_FILTERCONFIG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_filterConfig
{
public:

    void setupUi(QDialog *filterConfig)
    {
        if (filterConfig->objectName().isEmpty())
            filterConfig->setObjectName("filterConfig");
        filterConfig->resize(425, 332);

        retranslateUi(filterConfig);

        QMetaObject::connectSlotsByName(filterConfig);
    } // setupUi

    void retranslateUi(QDialog *filterConfig)
    {
        filterConfig->setWindowTitle(QCoreApplication::translate("filterConfig", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class filterConfig: public Ui_filterConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILTERCONFIG_H
