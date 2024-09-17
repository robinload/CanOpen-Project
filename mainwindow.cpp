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
    QColor(255, 0, 0),    // Red
    QColor(0, 255, 0),    // Green
    QColor(0, 0, 255),    // Blue
    QColor(255, 255, 0),  // Yellow
    QColor(255, 0, 255),  // Magenta
    QColor(0, 255, 255),  // Cyan
    QColor(128, 0, 0),    // Maroon
    QColor(0, 128, 0),    // Dark Green
    QColor(0, 0, 128),    // Navy
    QColor(128, 128, 0),  // Olive
    QColor(128, 0, 128),  // Purple
    QColor(0, 128, 128),  // Teal
    QColor(192, 192, 192),// Silver
    QColor(128, 128, 128),// Gray
    QColor(255, 165, 0),  // Orange
    QColor(75, 0, 130)    // Indigo
};

QColor MainWindow::getAvailableColor()
{
    for (const QColor &color : predefinedColors) {
        if (!usedColors.contains(color)) {
            usedColors.append(color);
            return color;
        }
    }

    // Recycle colors
    if (!predefinedColors.isEmpty()) {
        QColor color = usedColors.isEmpty() ? predefinedColors.first() : usedColors.takeFirst();
        usedColors.append(color);
        return color;
    }
    return QColor(); // Fallback color
}

void MainWindow::openConnectPopup()
{
    connectPopup *popup = new connectPopup(globalTimer,this);
    popup->setWindowTitle("Connect Popup");
    popup->resize(650, 500);

    connect(popup, &connectPopup::loadCellConnected, this, &MainWindow::addLoadCellCard);

    popup->show();
}


void MainWindow::addLoadCellCard(LoadCell *loadCell)
{
    int cardIndex = cardModel->rowCount();
    loadCells.append(loadCell);

    QColor color = getAvailableColor();
    int canId = loadCell->getCanId(); // Fetch the CAN ID from LoadCell
    cardModel->addCard(QStringList() << QString("CAN ID %1").arg(canId), 0, color);

    if (cardIndex >= ui->mainPlot->graphCount()) {
        ui->mainPlot->addGraph();
    }

    // Set the graph's pen color and name
    ui->mainPlot->graph(cardIndex)->setPen(QPen(color));
    ui->mainPlot->graph(cardIndex)->setName(QString("CAN ID %1").arg(canId)); // Set CAN ID as the graph name

    connect(loadCell, &LoadCell::weightChanged, [this, cardIndex](double weight) {
        if (isSampling) {  // Ensure data is processed only when sampling is active
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
    qDebug() << "Entered removeCard()";

    QModelIndex selectedIndex = listView->currentIndex();
    if (selectedIndex.isValid()) {
        int row = selectedIndex.row();
        if (row >= 0 && row < loadCells.size()) {
            qDebug() << "Removing LoadCell at Row:" << row;

            QColor color = cardModel->data(cardModel->index(row), Qt::DecorationRole).value<QColor>();
            usedColors.removeOne(color);

            delete loadCells.takeAt(row);
            cardModel->removeCard(row);

            int graphCount = ui->mainPlot->graphCount();
            if (row < graphCount) {
                qDebug() << "Clearing Graph Data at Index:" << row;
                ui->mainPlot->graph(row)->data()->clear();

                for (int i = row; i < ui->mainPlot->graphCount(); ++i) {
                    if (i < loadCells.size()) {
                        loadCells[i]->setIndex(i);
                        ui->mainPlot->graph(i)->setName(cardModel->cardName(i));
                        ui->mainPlot->graph(i)->setPen(QPen(cardModel->data(cardModel->index(i), Qt::DecorationRole).value<QColor>()));
                    }
                }

                ui->mainPlot->rescaleAxes();
                ui->mainPlot->replot();
            } else {
                qDebug() << "Attempted to remove graph with invalid index" << row;
            }
        } else {
            qDebug() << "Attempted to remove card with invalid index" << row;
        }
    } else {
        qDebug() << "No card selected for removal.";
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    listView(new QListView(this)),
    cardModel(new CardModel(this)), // Initialize based on the checkbox state
    autoScaleEnabled(false),
    globalTimer(new QTimer(this)),  // Initialize globalTimer
    manualRead(new ManualRead())   // Initialize the ManualRead instance
{
    ui->setupUi(this);
    this->resize(1350, 768); // Adjust as needed
    ui->mainPlot->clearGraphs();

    // Configure X-Axis to show system time
    QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
    dateTimeTicker->setDateTimeFormat("hh:mm:ss");
    ui->mainPlot->xAxis->setTicker(dateTimeTicker);
    ui->mainPlot->xAxis->setLabel("Time");

    // Configure interactions and limits
    ui->mainPlot->setInteraction(QCP::iRangeDrag, true);    // Enable panning
    ui->mainPlot->setInteraction(QCP::iRangeZoom, true);    // Enable zooming
    ui->mainPlot->axisRect()->setRangeZoom(Qt::Horizontal); // Horizontal zoom only
    ui->mainPlot->axisRect()->setRangeDrag(Qt::Horizontal); // Horizontal drag only

    // Configure Y-Axis to show weight in grams by default
    ui->mainPlot->yAxis->setLabel("Weight (g)");
    ui->mainPlot->axisRect()->setupFullAxesBox();
    ui->mainPlot->yAxis->setRange(-1.2, 1.2); // Default range

    // Synchronize the top and right axes with the bottom and left axes
    connect(ui->mainPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->mainPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->mainPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->mainPlot->yAxis2, SLOT(setRange(QCPRange)));

    // Timer for real-time data
    connect(globalTimer, &QTimer::timeout, this, &MainWindow::realtimeDataSlot);
    globalTimer->start(100); // Set the interval to 10 ms

    // ListView setup
    listView->setModel(cardModel);
    ui->addButton->setEnabled(false);
    CardDelegate *cardDelegate = new CardDelegate(this);
    listView->setItemDelegate(cardDelegate);
    ui->verticalLayout->addWidget(listView);
    connect(cardDelegate, &CardDelegate::zeroButtonClicked, this, &MainWindow::zeroButtonClicked);
    connect(cardDelegate, &CardDelegate::restoreButtonClicked, this, &MainWindow::restoreButtonClicked);
    // Connect buttons and checkboxes
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::openConnectPopup);
    connect(ui->advancedCommandButton, &QPushButton::clicked, this, &MainWindow::openAdvancedDialog);
    connect(ui->removeButton, &QPushButton::clicked, this, &MainWindow::removeCard);
    connect(ui->CSVButton, &QPushButton::clicked, this, &MainWindow::openSamplingPage);
    connect(ui->autoBox, &QCheckBox::toggled, this, &MainWindow::toggleAutoScale); // Auto-scale checkbox

    connect(ui->gramCheckbox, &QCheckBox::toggled, this, &MainWindow::updatePlotUnits);
    connect(ui->poundCheckbox, &QCheckBox::toggled, this, &MainWindow::updatePlotUnits);
    connect(ui->kilogramCheckbox, &QCheckBox::toggled, this, &MainWindow::updatePlotUnits);

    // Connect start/stop sampling buttons
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

    // Ensure only one unit checkbox is checked at a time
    QButtonGroup *unitButtonGroup = new QButtonGroup(this);
    unitButtonGroup->addButton(ui->gramCheckbox);
    unitButtonGroup->addButton(ui->poundCheckbox);
    unitButtonGroup->addButton(ui->kilogramCheckbox);
    unitButtonGroup->setExclusive(true);
    connect(unitButtonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, &MainWindow::updatePlotUnits);


    // Custom background color for plot area
    ui->mainPlot->setBackground(QBrush(QColor(240, 240, 240))); // Light gray background

    // Enable and customize the legend
    ui->mainPlot->legend->setVisible(true);
    ui->mainPlot->legend->setBrush(QBrush(QColor(255, 255, 255, 150))); // Semi-transparent white
    ui->mainPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);
}

void MainWindow::openAdvancedDialog()
{
    if (!isCANInitialized)
    {
        // Ensure CAN is connected before allowing access to the advanced dialog
        QMessageBox::warning(this, "CAN Not Connected", "Please connect to the CAN bus before accessing advanced settings.");
        return;
    }

    advancedDialog dialog(this);
    dialog.setWindowModality(Qt::ApplicationModal);
    dialog.exec();
}

void MainWindow::saveCalibrationSetting() {
    LoadCell *selectedLoadCell = getSelectedLoadCell();  // Returns a pointer to the selected LoadCell

    if (selectedLoadCell == nullptr) {
        QMessageBox::warning(this, "No Load Cell Selected", "Please select a load cell before saving calibration settings.");
        return;
    }

    selectedLoadCell->saveCalibrationData();
}

void MainWindow::openFilterConfig() {
    // Assuming you have a pointer to the selected LoadCell instance
    LoadCell *selectedLoadCell = getSelectedLoadCell();  // Returns a pointer to the selected LoadCell

    if (selectedLoadCell) {
        FilterConfig dialog(this, *selectedLoadCell);  // Dereference the pointer to pass the actual object
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
    return nullptr; // No valid selection
}

void MainWindow::zeroButtonClicked(const QModelIndex &index)
{
    qDebug() << "Entering zeroButtonClicked function with row:" << index.row();  // Debug message

    int row = index.row();

    if (row >= 0 && row < loadCells.size()) {
        int canId = loadCells[row]->getCanId();

        // Disconnect weight update
        loadCells[row]->disconnectUpdateWeight();

        // Use a timer to introduce a delay before sending the command
        QTimer::singleShot(1000, this, [this, canId, row]() {  // 100ms delay
            qDebug() << "Sending zero command to loadCell with CAN ID:" << canId;  // Debug message before sending

            // Define the command
            const std::vector<BYTE> zeroCommand = { 0x2F, 0x25, 0x20, 0x02, 0x01, 0x00, 0x00, 0x00 };

            // Send the command with the given CAN ID
            manualRead->SendMessages(zeroCommand, canId);

            qDebug() << "Zero command sent, reconnecting weight update for row:" << row;  // Debug after sending

            // Reconnect weight update after sending the command
            loadCells[row]->connectUpdateWeight();
        });
    } else {
        QMessageBox::warning(this, "Error", "Invalid card selected.");
        qDebug() << "Invalid card selection, row:" << row;  // Debug message for invalid selection
    }
}



void MainWindow::restoreButtonClicked(const QModelIndex &index)
{
    int row = index.row();

    if (row >= 0 && row < loadCells.size()) {
        int canId = loadCells[row]->getCanId();

        // Define the command
        const std::vector<BYTE> command = { 0x2F, 0x25, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00 };

        // Send the command with the given CAN ID
        manualRead->SendMessages(command, canId);
    } else {
        QMessageBox::warning(this, "Error", "Invalid card selected.");
        qDebug() << "Invalid card selection, row:" << row;
    }
}

void MainWindow::CANBusConnect()
{
    if (isCANInitialized)
    {
        // If CAN is already initialized, notify the user
        QMessageBox::information(this, "CAN Already Connected", "The CAN interface is already connected.");
        return;
    }

    TPCANStatus stsResult = CAN_Initialize(PcanHandle, Bitrate);

    if (stsResult == PCAN_ERROR_OK)
    {
        // Success case: Set flag to true and update the UI
        isCANInitialized = true;
        QMessageBox::information(this, "CAN Initialization", "CAN interface initialized successfully.");
        ui->statusLabel->setText("Status: Connected");
        ui->addButton->setEnabled(true);
    }
    else
    {
        // Failure case: Notify the user of an error
        QMessageBox::critical(this, "CAN Initialization Error", "Cannot initialize CAN. Please check your settings.");
    }
}
void MainWindow::CANBusDisconnect()
{
    if (!isCANInitialized)
    {
        // If the CAN bus is not initialized, inform the user
        QMessageBox::information(this, "CAN Already Disconnected", "The CAN interface is already disconnected.");
        return;
    }

    TPCANStatus stsResult = CAN_Uninitialize(PcanHandle);

    if (stsResult == PCAN_ERROR_OK)
    {
        // Success case: Update UI and state variables
        isCANInitialized = false;  // Set the initialization flag to false
        QMessageBox::information(this, "CAN Uninitialization", "CAN interface uninitialized successfully.");
        ui->statusLabel->setText("Status: Disconnected");
        isSampling = false;  // Ensure sampling is stopped
        ui->addButton->setEnabled(false);  // Disable buttons that require CAN connection
    }
    else
    {
        // Failure case: Notify the user
        QMessageBox::critical(this, "CAN Uninitialization Error", "Cannot uninitialize CAN. Please check your settings.");
    }
}


void MainWindow::updateYRange()
{
    if (ui->manualCheckbox->isChecked()) {
        // Read values from maxBox and minBox
        double minY = ui->minBox->value();
        double maxY = ui->maxBox->value();

        if (maxY > minY) {
            ui->mainPlot->yAxis->setRange(minY, maxY);
        } else {
            QMessageBox::warning(this, "Invalid Range", "Maximum value must be greater than minimum value.");
        }
    } else {
        // Reset to default range if manual mode is off
        ui->mainPlot->yAxis->setRange(-1.2, 1.2);
    }

    ui->mainPlot->replot(); // Redraw the plot with the new settings
}

void MainWindow::startSampling()
{
    startTime = QDateTime::currentDateTime(); // Initialize start time
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    isSampling = true;
}


void MainWindow::stopSampling()
{
    // dataTimer.stop();  // Stop the timer
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
        return weight; // Already in grams
    } else if (ui->poundCheckbox->isChecked()) {
        return weight / 453.592; // Convert grams to pounds
    } else if (ui->kilogramCheckbox->isChecked()) {
        return weight / 1000.0; // Convert grams to kilograms
    }
    return weight; // Fallback
}

void MainWindow::updatePlotUnits()
{
    // Determine which unit is selected and update the CardModel
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

    // Update the plots to reflect the new unit
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

        // Apply conversion to each data point
        for (auto it = dataContainer->begin(); it != dataContainer->end(); ++it) {
            it->value *= conversionFactor;
        }
    }

    ui->mainPlot->replot(); // Redraw the plot with the new settings
}

void MainWindow::realtimeDataSlot()
{
    if (!isSampling) return;

    QDateTime currentDateTime = QDateTime::currentDateTime();
    double key = currentDateTime.toMSecsSinceEpoch() / 1000.0;

    bool rescaleNeeded = false;
    double maxYValue = std::numeric_limits<double>::min(); // Track the highest y-axis value within the visible range

    double visibleRangeStart = key - 8;  // Assuming you want to show the last 8 seconds

    for (int i = 0; i < loadCells.size(); ++i) {
        if (i < ui->mainPlot->graphCount()) {
            if (cardModel->data(cardModel->index(i), Qt::CheckStateRole).toBool()) {
                double weight = convertWeightToSelectedUnit(loadCells[i]->currentWeight());
                ui->mainPlot->graph(i)->addData(key, weight);

                // Iterate over data points within the visible range to find the maximum Y value
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

    // Update x-axis range
    ui->mainPlot->xAxis->setRange(visibleRangeStart, key);

    // Initial y-axis range setting if no data is present
    if (maxYValue == std::numeric_limits<double>::min()) {
        ui->mainPlot->yAxis->setRange(0, 1); // Set a default y-axis range
    }

    if (rescaleNeeded && autoScaleEnabled) {
        double marginFactor = 0.1; // 10% margin
        double margin = maxYValue * marginFactor;
        ui->mainPlot->yAxis->setRange(0, maxYValue + margin); // Add margin to maxYValue
    }

    ui->mainPlot->replot();
}