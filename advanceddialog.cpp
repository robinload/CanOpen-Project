#include "advanceddialog.h"
#include "utility.h" // Include the header for ManualRead and SendMessages function
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSettings>
#include <sstream>
#include <iomanip>

advancedDialog::advancedDialog(QWidget *parent) :
    QDialog(parent),
    manualRead(new ManualRead()) // Initialize ManualRead instance
{
    setupUi();
    retranslateUi();

    // Connect sendButton click signal to the slot
    connect(sendButton, &QPushButton::clicked, this, &advancedDialog::onSendButtonClicked);
}

advancedDialog::~advancedDialog()
{
    delete manualRead; // Clean up
}

void advancedDialog::setupUi()
{
    setWindowTitle(tr("Dialog"));
    resize(400, 300);

    stackedWidget = new QStackedWidget(this);

    // Page 1
    page = new QWidget();
    QVBoxLayout *verticalLayoutPage1 = new QVBoxLayout(page);

    label = new QLabel(tr("Enter Password"), page);
    verticalLayoutPage1->addWidget(label);

    passwordLineEdit = new QLineEdit(page);
    verticalLayoutPage1->addWidget(passwordLineEdit);

    nextPageButton = new QPushButton(tr("Next"), page);
    verticalLayoutPage1->addWidget(nextPageButton);
    connect(nextPageButton, &QPushButton::clicked, this, &advancedDialog::onNextPageButtonClicked);

    stackedWidget->addWidget(page);

    // Page 2
    page_2 = new QWidget();
    QVBoxLayout *verticalLayoutPage2 = new QVBoxLayout(page_2);

    QLabel *label_2 = new QLabel(tr("CAN ID"), page_2);
    canIDLineEdit = new QLineEdit(page_2);
    QHBoxLayout *canIDLayout = new QHBoxLayout();
    canIDLayout->addWidget(label_2);
    canIDLayout->addWidget(canIDLineEdit);

    verticalLayoutPage2->addLayout(canIDLayout);

    label_3 = new QLabel(tr("Data:"), page_2);
    verticalLayoutPage2->addWidget(label_3);

    QWidget *horizontalLayoutWidget = new QWidget(page_2);
    QHBoxLayout *horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);

    byte1Edit = new QLineEdit(horizontalLayoutWidget);
    byte2Edit = new QLineEdit(horizontalLayoutWidget);
    byte3Edit = new QLineEdit(horizontalLayoutWidget);
    byte4Edit = new QLineEdit(horizontalLayoutWidget);
    byte5Edit = new QLineEdit(horizontalLayoutWidget);
    byte6Edit = new QLineEdit(horizontalLayoutWidget);
    byte7Edit = new QLineEdit(horizontalLayoutWidget);
    byte8Edit = new QLineEdit(horizontalLayoutWidget);

    horizontalLayout->addWidget(byte1Edit);
    horizontalLayout->addWidget(byte2Edit);
    horizontalLayout->addWidget(byte3Edit);
    horizontalLayout->addWidget(byte4Edit);
    horizontalLayout->addWidget(byte5Edit);
    horizontalLayout->addWidget(byte6Edit);
    horizontalLayout->addWidget(byte7Edit);
    horizontalLayout->addWidget(byte8Edit);

    verticalLayoutPage2->addWidget(horizontalLayoutWidget);

    sendButton = new QPushButton(tr("Send"), page_2);
    verticalLayoutPage2->addWidget(sendButton);

    stackedWidget->addWidget(page_2);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(stackedWidget);
    setLayout(mainLayout);
}

void advancedDialog::retranslateUi()
{
    // This method can be used for translations if needed
}

void advancedDialog::onNextPageButtonClicked()
{
    // Load password from config file or perform your logic here
    QSettings settings("config.ini", QSettings::IniFormat);
    QString configuredPassword = "1234";

    if (passwordLineEdit->text() == configuredPassword) {
        stackedWidget->setCurrentWidget(page_2);
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Incorrect password."));
    }
}


void advancedDialog::onSendButtonClicked()
{
    bool ok;
    uint32_t canId = canIDLineEdit->text().toUInt(&ok, 16); // Read CAN ID from the input field
    if (!ok) {
        QMessageBox::warning(this, tr("Error"), tr("Invalid CAN ID."));
        return;
    }

    std::vector<BYTE> data;
    data.push_back(static_cast<BYTE>(byte1Edit->text().toUInt(&ok, 16)));
    data.push_back(static_cast<BYTE>(byte2Edit->text().toUInt(&ok, 16)));
    data.push_back(static_cast<BYTE>(byte3Edit->text().toUInt(&ok, 16)));
    data.push_back(static_cast<BYTE>(byte4Edit->text().toUInt(&ok, 16)));
    data.push_back(static_cast<BYTE>(byte5Edit->text().toUInt(&ok, 16)));
    data.push_back(static_cast<BYTE>(byte6Edit->text().toUInt(&ok, 16)));
    data.push_back(static_cast<BYTE>(byte7Edit->text().toUInt(&ok, 16)));
    data.push_back(static_cast<BYTE>(byte8Edit->text().toUInt(&ok, 16)));

    if (!ok) {
        QMessageBox::warning(this, tr("Error"), tr("Invalid data values."));
        return;
    }

    // Send the message
    manualRead->SendMessages(data, canId);

    // Calculate the expected return CAN ID
    uint32_t expectedReturnId = canId - 0x80; // Subtract 0x80 (128 in decimal)

    TPCANStatus status;
    UINT32 responseId;
    std::vector<BYTE> rxCommand;
    int attempts = 0;
    const int maxAttempts = 20;

    // Try reading the correct response up to 20 times
    do {
        std::tuple<TPCANStatus, UINT32, std::vector<BYTE>> result = manualRead->ReadMessage();
        status = std::get<0>(result);
        responseId = std::get<1>(result);
        rxCommand = std::get<2>(result);

        attempts++;
    } while (responseId != expectedReturnId && attempts < maxAttempts);

    QString message;

    if (responseId == expectedReturnId) {
        // If the correct response is received, format the sent and received data for display
        message.append(tr("Command sent: "));
        for (BYTE byte : data) {
            message.append(QString("%1 ").arg(byte, 2, 16, QLatin1Char('0')).toUpper()); // Format bytes in hex
        }
        message.append(tr("\nCAN ID: %1").arg(canId, 8, 16, QLatin1Char('0')).toUpper()); // Format CAN ID in hex

        message.append(tr("\n\nResponse received:\n"));
        message.append(tr("Response ID: %1\n").arg(responseId, 8, 16, QLatin1Char('0')).toUpper());

        message.append("Data: ");
        for (BYTE byte : rxCommand) {
            message.append(QString("%1 ").arg(byte, 2, 16, QLatin1Char('0')).toUpper());
        }
    } else {
        // If no correct response is received after 20 attempts, show an error message
        message.append(tr("Error: Failed to receive the expected response within 20 attempts.\n"));
        message.append(tr("Expected Response ID: %1\n").arg(expectedReturnId, 8, 16, QLatin1Char('0')).toUpper());
        message.append(tr("Last Response ID received: %1").arg(responseId, 8, 16, QLatin1Char('0')).toUpper());
    }

    // Show the QMessageBox with the information
    QMessageBox::information(this, tr("Command Sent"), message);
}
