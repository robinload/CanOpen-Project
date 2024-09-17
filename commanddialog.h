#ifndef COMMANDDIALOG_H
#define COMMANDDIALOG_H

#include <QDialog>

namespace Ui {
class CommandDialog;
}

class CommandDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CommandDialog(QWidget *parent = nullptr);
    ~CommandDialog();

private slots:
    void on_submitButton_clicked();
    void on_sendCommandButton_clicked();

private:
    Ui::CommandDialog *ui;
    QString getCommand(); // Helper function to get the full command
};

#endif // COMMANDDIALOG_H
