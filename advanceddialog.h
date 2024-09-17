#ifndef ADVANCEDDIALOG_H
#define ADVANCEDDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>
#include <vector>

class ManualRead; // Forward declaration of ManualRead class

class advancedDialog : public QDialog
{
    Q_OBJECT

public:
    explicit advancedDialog(QWidget *parent = nullptr);
    ~advancedDialog();

private slots:
    void onNextPageButtonClicked();
    void onSendButtonClicked(); // Slot for handling sendButton click

private:
    void setupUi();
    void retranslateUi();

    QStackedWidget *stackedWidget;
    QWidget *page;
    QWidget *page_2;

    QLineEdit *passwordLineEdit;
    QPushButton *nextPageButton;
    QLabel *label;

    QLineEdit *canIDLineEdit;
    QLineEdit *byte1Edit;
    QLineEdit *byte2Edit;
    QLineEdit *byte3Edit;
    QLineEdit *byte4Edit;
    QLineEdit *byte5Edit;
    QLineEdit *byte6Edit;
    QLineEdit *byte7Edit;
    QLineEdit *byte8Edit;
    QPushButton *sendButton; // New button

    QLabel *label_3;

    ManualRead *manualRead; // Pointer to ManualRead instance
};

#endif // ADVANCEDDIALOG_H
