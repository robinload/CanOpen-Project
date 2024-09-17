#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QSettings>

class ConfigDialog : public QDialog {
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = nullptr);
    ~ConfigDialog();

    int getBaudRate() const;
    QString getInterface() const;
    QString getMode() const;

    void loadSettings();
    void saveSettings() const;

private:
    QComboBox *baudRateComboBox;
    QComboBox *interfaceComboBox;
    QComboBox *modeComboBox;
};

#endif // CONFIGDIALOG_H
