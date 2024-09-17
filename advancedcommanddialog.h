#ifndef ADVANCEDCOMMANDDIALOG_H
#define ADVANCEDCOMMANDDIALOG_H

#include <QDialog>
#include <QStackedWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>>
#include <QLabel>

class advancedCommandDialog : public QDialog
{
    Q_OBJECT

public:
    explicit advancedCommandDialog(QWidget *parent = nullptr);
    ~advancedCommandDialog();

private slots:
    void onNextButtonClicked();
    void onSubmitButtonClicked();

private:
    void setupUi();
    void retranslateUi();

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
};

#endif // ADVANCEDCOMMANDDIALOG_H
