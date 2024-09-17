#include "calibrationpopup.h"
#include "ui_calibrationpopup.h"

calibrationPopup::calibrationPopup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::calibrationPopup)
{
    ui->setupUi(this);
}

calibrationPopup::~calibrationPopup()
{
    delete ui;
}
