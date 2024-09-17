#ifndef CALIBRATIONPOPUP_H
#define CALIBRATIONPOPUP_H

#include <QWidget>

namespace Ui {
class calibrationPopup;
}

class calibrationPopup : public QWidget
{
    Q_OBJECT

public:
    explicit calibrationPopup(QWidget *parent = nullptr);
    ~calibrationPopup();

private:
    Ui::calibrationPopup *ui;
};

#endif // CALIBRATIONPOPUP_H
