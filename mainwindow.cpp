#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include "cardmodel.h"
#include "carddelegate.h"
#include "loadcell.h"
#include <QElapsedTimer>
#include <QRandomGenerator>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include "connectpopup.h"
#include "samplingpage.h"
#include "advanceddialog.h"
#include "utility.h"
#include "filterconfig.h"

const QList<QColor> predefinedColors = {
    QColor(255, 0, 0),
    QColor(0, 255, 0),
    QColor(0, 0, 255),
    QColor(255, 255, 0),
    QColor(255, 0, 255),
    QColor(0, 255, 255),
    QColor(128, 0, 0),
    QColor(0, 128, 0),
    QColor(0, 0, 128),
    QColor(128, 128, 0),
    QColor(128, 0, 128),
    QColor(0, 128, 128),
    QColor(192, 192, 192),
    QColor(128, 128, 128),
    QColor(255, 165, 0),
    QColor(75, 0, 130)
};

QColor MainWindow::getAvailableColor()
{
    for (const QColor &color : predefinedColors) {
        if (!usedColors.contains(color))
        {
            usedColors.append(color);
            return color;
        }
    }

    if (!predefinedColors.isEmpty()) {
        QColor color = usedColors.isEmpty() ? predefinedColors.first() : usedColors.takeFirst();
        usedColors.append(color);
        return color;
    }
    return QColor();
}

void MainWindow::openConnectPopup()
{
    connectPopup *popup = new connectPopup(globalTimer,this);
    popup->setWindowTitle("Connect Popup");
    popup->resize(650, 500);

    connect(popup, &connectPopup::loadCellConnected, this, &MainWindow::addLoadCellCard);

    popup->show();
}

QList<int> MainWindow::getAllCanIds() {

    QList<int> canIds;

    for (int i = 0; i < loadCells.size(); ++i) {
        LoadCell *loadCell = loadCells.at(i);

        if (loadCell) {
            int canId = loadCell->getCanId();
            canIds.push_back(canId);
        }
    }

    return canIds;
}

void MainWindow::addLoadCellCard(LoadCell *loadCell)
{
    int cardIndex = cardModel->rowCount();
    loadCells.append(loadCell);

    QColor color = getAvailableColor();
    int canId = loadCell->getCanId();
    uint8_t originalCanId = canId & 0xFF;
    cardModel->addCard(QStringList() << QString("CAN ID %1").arg(originalCanId), 0, color);

    if (cardIndex >= ui->mainPlot->graphCount()) {
        ui->mainPlot->addGraph();
    }

    ui->mainPlot->graph(cardIndex)->setPen(QPen(color));
    ui->mainPlot->graph(cardIndex)->setName(QString("CAN ID %1").arg(originalCanId));

    connect(loadCell, &LoadCell::weightChanged, [this, cardIndex](double weight) {
        if (isSampling) {
            double key = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;

            if (cardModel->data(cardModel->index(cardIndex), Qt::CheckStateRole).toBool()) {
                double adjustedWeight = convertWeightToSelectedUnit(weight);
                ui->mainPlot->graph(cardIndex)->addData(key, adjustedWeight);
                ui->mainPlot->xAxis->setRange(key, 8, Qt::AlignRight);
                ui->mainPlot->replot();
            }
        }
        cardModel->updateCard(cardIndex, weight);
    });
}

QList<LoadCell*> MainWindow::getLoadCells() const
{
    return loadCells;
}

void MainWindow::openSamplingPage()
{
    SamplingPage *samplingPage = new SamplingPage(nullptr, this);
    samplingPage->setWindowModality(Qt::ApplicationModal);
    samplingPage->show();
}

void MainWindow::removeCard()
{
    QModelIndex selectedIndex = listView->currentIndex();
    if (selectedIndex.isValid()) {
        int row = selectedIndex.row();
        if (row >= 0 && row < loadCells.size()) {
            QColor color = cardModel->data(cardModel->index(row), Qt::DecorationRole).value<QColor>();
            usedColors.removeOne(color);

            delete loadCells.takeAt(row);
            cardModel->removeCard(row);

            if (row < ui->mainPlot->graphCount()) {
                ui->mainPlot->removeGraph(row);

                for (int i = row; i < ui->mainPlot->graphCount(); ++i) {
                    loadCells[i]->setIndex(i);
                    ui->mainPlot->graph(i)->setName(cardModel->cardName(i));
                    ui->mainPlot->graph(i)->setPen(QPen(cardModel->data(cardModel->index(i), Qt::DecorationRole).value<QColor>()));
                }

                ui->mainPlot->replot();
            }
        }
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    listView(new QListView(this)),
    cardModel(new CardModel(this)),
    autoScaleEnabled(false),
    globalTimer(new QTimer(this)),
    manualRead(new ManualRead()),
    loadCells()
{
    ui->setupUi(this);
    this->resize(1350, 768);
    ui->mainPlot->clearGraphs();

    QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
    dateTimeTicker->setDateTimeFormat("hh:mm:ss");
    ui->mainPlot->xAxis->setTicker(dateTimeTicker);
    ui->mainPlot->xAxis->setLabel("Time");

    ui->mainPlot->setInteraction(QCP::iRangeDrag, true);
    ui->mainPlot->setInteraction(QCP::iRangeZoom, true);
    ui->mainPlot->axisRect()->setRangeZoom(Qt::Horizontal);
    ui->mainPlot->axisRect()->setRangeDrag(Qt::Horizontal);

    ui->mainPlot->yAxis->setLabel("Weight (g)");
    ui->mainPlot->axisRect()->setupFullAxesBox();
    ui->mainPlot->yAxis->setRange(-1.2, 1.2);

    connect(ui->mainPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->mainPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->mainPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->mainPlot->yAxis2, SLOT(setRange(QCPRange)));

    connect(globalTimer, &QTimer::timeout, this, &MainWindow::realtimeDataSlot);
    globalTimer->start(100);

    listView->setModel(cardModel);
    ui->addButton->setEnabled(false);
    CardDelegate *cardDelegate = new CardDelegate(this);
    listView->setItemDelegate(cardDelegate);
    ui->verticalLayout->addWidget(listView);
    connect(cardDelegate, &CardDelegate::zeroButtonClicked, this, &MainWindow::zeroButtonClicked);
    connect(cardDelegate, &CardDelegate::restoreButtonClicked, this, &MainWindow::restoreButtonClicked);
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::openConnectPopup);
    connect(ui->advancedCommandButton, &QPushButton::clicked, this, &MainWindow::openAdvancedDialog);
    connect(ui->removeButton, &QPushButton::clicked, this, &MainWindow::removeCard);
    connect(ui->CSVButton, &QPushButton::clicked, this, &MainWindow::openSamplingPage);
    connect(ui->autoBox, &QCheckBox::toggled, this, &MainWindow::toggleAutoScale);

    connect(ui->gramCheckbox, &QCheckBox::toggled, this, &MainWindow::updatePlotUnits);
    connect(ui->poundCheckbox, &QCheckBox::toggled, this, &MainWindow::updatePlotUnits);
    connect(ui->kilogramCheckbox, &QCheckBox::toggled, this, &MainWindow::updatePlotUnits);

    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startSampling);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopSampling);
    connect(ui->filterConfigButton, &QPushButton::clicked, this, &MainWindow::openFilterConfig);
    connect(ui->saveCalibrationButton, &QPushButton::clicked, this, &MainWindow::saveCalibrationSetting);

    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::CANBusConnect);
    connect(ui->disconnectButton, &QPushButton::clicked, this, &MainWindow::CANBusDisconnect);

    connect(ui->manualCheckbox, &QCheckBox::toggled, this, &MainWindow::updateYRange);
    connect(ui->maxBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::updateYRange);
    connect(ui->minBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::updateYRange);
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    isSampling = false;

    QButtonGroup *unitButtonGroup = new QButtonGroup(this);
    unitButtonGroup->addButton(ui->gramCheckbox);
    unitButtonGroup->addButton(ui->poundCheckbox);
    unitButtonGroup->addButton(ui->kilogramCheckbox);
    unitButtonGroup->setExclusive(true);
    connect(unitButtonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, &MainWindow::updatePlotUnits);

    ui->mainPlot->setBackground(QBrush(QColor(240, 240, 240)));

    ui->mainPlot->legend->setVisible(true);
    ui->mainPlot->legend->setBrush(QBrush(QColor(255, 255, 255, 150)));
    ui->mainPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);
}

void MainWindow::openAdvancedDialog()
{
    if (!isCANInitialized)
    {
        QMessageBox::warning(this, "CAN Not Connected", "Please connect to the CAN bus before accessing advanced settings.");
        return;
    }

    advancedDialog dialog(this);
    dialog.setWindowModality(Qt::ApplicationModal);
    dialog.exec();
}

void MainWindow::saveCalibrationSetting() {
    LoadCell *selectedLoadCell = getSelectedLoadCell();

    if (selectedLoadCell == nullptr) {
        QMessageBox::warning(this, "No Load Cell Selected", "Please select a load cell before saving calibration settings.");
        return;
    }

    selectedLoadCell->saveCalibrationData();
}

void MainWindow::openFilterConfig() {
    LoadCell *selectedLoadCell = getSelectedLoadCell();

    if (selectedLoadCell) {
        FilterConfig dialog(this, *selectedLoadCell);
        dialog.setWindowModality(Qt::ApplicationModal);
        dialog.exec();
    } else {
        QMessageBox::warning(this, "No Load Cell Selected", "Please select a load cell before configuring.");
    }
}

LoadCell* MainWindow::getSelectedLoadCell() const
{
    QModelIndex selectedIndex = listView->currentIndex();
    if (selectedIndex.isValid()) {
        int row = selectedIndex.row();
        if (row >= 0 && row < loadCells.size()) {
            return loadCells[row];
        }
    }
    return nullptr;
}

void MainWindow::zeroButtonClicked(const QModelIndex &index)
{
    int row = index.row();

    if (row >= 0 && row < loadCells.size()) {
        int canId = loadCells[row]->getCanId();

        loadCells[row]->disconnectUpdateWeight();

        QTimer::singleShot(100, this, [this, canId, row]() {
            const std::vector<BYTE> zeroCommand = { 0x2F, 0x25, 0x20, 0x02, 0x01, 0x00, 0x00, 0x00 };
            manualRead->SendMessages(zeroCommand, canId);
            loadCells[row]->connectUpdateWeight();
        });
    } else {
        QMessageBox::warning(this, "Error", "Invalid card selected.");
    }
}

void MainWindow::restoreButtonClicked(const QModelIndex &index)
{
    int row = index.row();

    if (row >= 0 && row < loadCells.size()) {
        int canId = loadCells[row]->getCanId();

        const std::vector<BYTE> command = { 0x2F, 0x25, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00 };

        manualRead->SendMessages(command, canId);
    } else {
        QMessageBox::warning(this, "Error", "Invalid card selected.");
    }
}

void MainWindow::CANBusConnect()
{
    if (isCANInitialized)
    {
        QMessageBox::information(this, "CAN Already Connected", "The CAN interface is already connected.");
        return;
    }

    TPCANStatus stsResult = CAN_Initialize(PcanHandle, Bitrate);

    if (stsResult == PCAN_ERROR_OK)
    {
        isCANInitialized = true;
        QMessageBox::information(this, "CAN Initialization", "CAN interface initialized successfully.");
        ui->statusLabel->setText("Status: Connected");
        ui->addButton->setEnabled(true);
    }
    else
    {
        QMessageBox::critical(this, "CAN Initialization Error", "Cannot initialize CAN. Please check your settings.");
    }
}

void MainWindow::CANBusDisconnect()
{
    if (!isCANInitialized)
    {
        QMessageBox::information(this, "CAN Already Disconnected", "The CAN interface is already disconnected.");
        return;
    }

    TPCANStatus stsResult = CAN_Uninitialize(PcanHandle);

    if (stsResult == PCAN_ERROR_OK)
    {
        isCANInitialized = false;
        QMessageBox::information(this, "CAN Uninitialization", "CAN interface uninitialized successfully.");
        ui->statusLabel->setText("Status: Disconnected");
        isSampling = false;
        ui->addButton->setEnabled(false);
    }
    else
    {
        QMessageBox::critical(this, "CAN Uninitialization Error", "Cannot uninitialize CAN. Please check your settings.");
    }
}

void MainWindow::updateYRange()
{
    if (ui->manualCheckbox->isChecked()) {
        double minY = ui->minBox->value();
        double maxY = ui->maxBox->value();

        if (maxY > minY) {
            ui->mainPlot->yAxis->setRange(minY, maxY);
        } else {
            QMessageBox::warning(this, "Invalid Range", "Maximum value must be greater than minimum value.");
        }
    } else {
        ui->mainPlot->yAxis->setRange(-1.2, 1.2);
    }

    ui->mainPlot->replot();
}

void MainWindow::startSampling()
{
    startTime = QDateTime::currentDateTime();
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    isSampling = true;
}

void MainWindow::stopSampling()
{
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    isSampling = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::toggleAutoScale(bool checked)
{
    autoScaleEnabled = checked;
}

double MainWindow::convertWeightToSelectedUnit(double weight)
{
    if (ui->gramCheckbox->isChecked()) {
        return weight;
    } else if (ui->poundCheckbox->isChecked()) {
        return weight / 453.592;
    } else if (ui->kilogramCheckbox->isChecked()) {
        return weight / 1000.0;
    }
    return weight;
}

void MainWindow::updatePlotUnits()
{
    if (ui->gramCheckbox->isChecked()) {
        ui->poundCheckbox->setChecked(false);
        ui->kilogramCheckbox->setChecked(false);
        ui->mainPlot->yAxis->setLabel("Weight (g)");
        cardModel->setWeightUnit(CardModel::Grams);

    } else if (ui->poundCheckbox->isChecked()) {
        ui->gramCheckbox->setChecked(false);
        ui->kilogramCheckbox->setChecked(false);
        ui->mainPlot->yAxis->setLabel("Weight (lb)");
        cardModel->setWeightUnit(CardModel::Pounds);

    } else if (ui->kilogramCheckbox->isChecked()) {
        ui->gramCheckbox->setChecked(false);
        ui->poundCheckbox->setChecked(false);
        ui->mainPlot->yAxis->setLabel("Weight (kg)");
        cardModel->setWeightUnit(CardModel::Kilograms);
    }

    for (int i = 0; i < ui->mainPlot->graphCount(); ++i) {
        QCPGraph *graph = ui->mainPlot->graph(i);
        QSharedPointer<QCPGraphDataContainer> dataContainer = graph->data();

        double conversionFactor = 1.0;
        switch (cardModel->getWeightUnit()) {
        case CardModel::Grams:
            conversionFactor = 1.0;
            break;
        case CardModel::Pounds:
            conversionFactor = 1.0 / 453.592;
            break;
        case CardModel::Kilograms:
            conversionFactor = 1.0 / 1000.0;
            break;
        }

        for (auto it = dataContainer->begin(); it != dataContainer->end(); ++it) {
            it->value *= conversionFactor;
        }
    }

    ui->mainPlot->replot();
}

void MainWindow::realtimeDataSlot()
{
    if (!isSampling) return;

    QDateTime currentDateTime = QDateTime::currentDateTime();
    double key = currentDateTime.toMSecsSinceEpoch() / 1000.0;

    bool rescaleNeeded = false;
    double maxYValue = std::numeric_limits<double>::min();

    double visibleRangeStart = key - 8;

    for (int i = 0; i < loadCells.size(); ++i) {
        if (i < ui->mainPlot->graphCount()) {
            if (cardModel->data(cardModel->index(i), Qt::CheckStateRole).toBool()) {
                double weight = convertWeightToSelectedUnit(loadCells[i]->currentWeight());
                ui->mainPlot->graph(i)->addData(key, weight);

                QCPGraphDataContainer::const_iterator it = ui->mainPlot->graph(i)->data()->constBegin();
                while (it != ui->mainPlot->graph(i)->data()->constEnd()) {
                    if (it->key >= visibleRangeStart && it->key <= key) {
                        if (it->value > maxYValue) {
                            maxYValue = it->value;
                            rescaleNeeded = true;
                        }
                    }
                    ++it;
                }
            }
        }
    }

    ui->mainPlot->xAxis->setRange(visibleRangeStart, key);

    if (maxYValue == std::numeric_limits<double>::min()) {
        ui->mainPlot->yAxis->setRange(0, 1);
    }

    if (rescaleNeeded && autoScaleEnabled) {
        double marginFactor = 0.1;
        double margin = maxYValue * marginFactor;
        ui->mainPlot->yAxis->setRange(0, maxYValue + margin);
    }

    ui->mainPlot->replot();
}
