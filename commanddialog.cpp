#include "commanddialog.h"
#include "ui_CommandDialog.h"
#include <QMessageBox>

CommandDialog::CommandDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommandDialog)
{
    ui->setupUi(this);
    // Initially show the password page
    ui->stackedWidget->setCurrentIndex(0);
}

CommandDialog::~CommandDialog()
{
    delete ui;
}

void CommandDialog::on_submitButton_clicked()
{
    // Simple password check (replace with your own logic)
    const QString correctPassword = "password123"; // Replace with your password logic
    if (ui->passwordLineEdit->text() == correctPassword) {
        ui->stackedWidget->setCurrentIndex(1); // Switch to command page
    } else {
        // Handle incorrect password
        QMessageBox::warning(this, "Error", "Incorrect password!");
    }
}

void CommandDialog::on_sendCommandButton_clicked()
{
    QString command = getCommand();
    if (!command.isEmpty()) {
        // Send the command through CANBUS
        // Example: sendCommand(command); // Replace with your CANBUS sending logic
        QMessageBox::information(this, "Success", "Command sent: " + command);
    } else {
        QMessageBox::warning(this, "Error", "Invalid command!");
    }
}

QString CommandDialog::getCommand()
{
    QString command;
    command.append(ui->byte1LineEdit->text().toUpper());
    command.append(ui->byte2LineEdit->text().toUpper());
    command.append(ui->byte3LineEdit->text().toUpper());
    command.append(ui->byte4LineEdit->text().toUpper());
    command.append(ui->byte5LineEdit->text().toUpper());
    return command;
}
