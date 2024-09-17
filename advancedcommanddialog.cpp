#include "advancedcommanddialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QLineEdit>
#include <QMessageBox>

advancedCommandDialog::advancedCommandDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi();
    retranslateUi();
}

advancedCommandDialog::~advancedCommandDialog() {}

void advancedCommandDialog::setupUi()
{
    setWindowTitle(tr("Advanced Command Dialog"));
    resize(400, 300);

    stackedWidget = new QStackedWidget(this);

    // Page 1
    page1 = new QWidget();
    verticalLayout = new QVBoxLayout(page1);

    passwordLabel = new QLabel(tr("Password:"), page1);
    verticalLayout->addWidget(passwordLabel);

    passwordLineEdit = new QLineEdit(page1);
    verticalLayout->addWidget(passwordLineEdit);

    nextButton = new QPushButton(tr("Next"), page1);
    verticalLayout->addWidget(nextButton);
    connect(nextButton, &QPushButton::clicked, this, &advancedCommandDialog::onNextButtonClicked);

    stackedWidget->addWidget(page1);

    // Page 2
    page2 = new QWidget();
    verticalLayout_2 = new QVBoxLayout(page2);

    QLabel *page2Title = new QLabel(tr("Page 2"), page2);
    verticalLayout_2->addWidget(page2Title);

    idLabel = new QLabel(tr("ID:"), page2);
    verticalLayout_2->addWidget(idLabel);

    idLineEdit = new QLineEdit(page2);
    verticalLayout_2->addWidget(idLineEdit);

    dataLabel = new QLabel(tr("Data Bytes (2 digits each):"), page2);
    verticalLayout_2->addWidget(dataLabel);

    dataByte1LineEdit = new QLineEdit(page2);
    verticalLayout_2->addWidget(dataByte1LineEdit);

    dataByte2LineEdit = new QLineEdit(page2);
    verticalLayout_2->addWidget(dataByte2LineEdit);

    dataByte3LineEdit = new QLineEdit(page2);
    verticalLayout_2->addWidget(dataByte3LineEdit);

    dataByte4LineEdit = new QLineEdit(page2);
    verticalLayout_2->addWidget(dataByte4LineEdit);

    dataByte5LineEdit = new QLineEdit(page2);
    verticalLayout_2->addWidget(dataByte5LineEdit);

    submitButton = new QPushButton(tr("Submit"), page2);
    verticalLayout_2->addWidget(submitButton);
    connect(submitButton, &QPushButton::clicked, this, &advancedCommandDialog::onSubmitButtonClicked);

    stackedWidget->addWidget(page2);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(stackedWidget);
    setLayout(mainLayout);
}

void advancedCommandDialog::retranslateUi()
{
    // This method is not strictly necessary for translations if using Qt's automatic
    // translation mechanisms, but you can use it if needed.
}

void advancedCommandDialog::onNextButtonClicked()
{
    // Basic password check (replace "password" with actual password)
    if (passwordLineEdit->text() == "password") {
        stackedWidget->setCurrentWidget(page2);
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Incorrect password."));
    }
}

void advancedCommandDialog::onSubmitButtonClicked()
{
    // Retrieve data and perform necessary actions
    QString id = idLineEdit->text();
    QString dataByte1 = dataByte1LineEdit->text();
    QString dataByte2 = dataByte2LineEdit->text();
    QString dataByte3 = dataByte3LineEdit->text();
    QString dataByte4 = dataByte4LineEdit->text();
    QString dataByte5 = dataByte5LineEdit->text();

    // Validate and process the data here

    QMessageBox::information(this, tr("Success"), tr("Data submitted successfully."));
}
