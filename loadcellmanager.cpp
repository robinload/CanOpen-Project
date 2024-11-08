#include "loadcellmanager.h"
#include <vector>
#include "globals.h"


LoadCellManager::LoadCellManager(ManualRead* reader, QObject *parent)
    : QObject(parent), manualRead(reader)
{
    // Initialize CAN ID to internal ID mappings
    canIdToInternalIdMap[601] = 581;

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &LoadCellManager::onTimerTimeout);
    updateTimer->start(10);  // Update interval in milliseconds (e.g., 1000ms = 1s)
}

LoadCellManager::~LoadCellManager()
{
    delete updateTimer;
}

void LoadCellManager::updateAllLoadCells()
{
    for (const auto& [canId, internalId] : canIdToInternalIdMap)
    {
        updateWeight(canId);
        updateADCValue(canId);
    }
}

void LoadCellManager::updateWeight(int canId)
{
    // Prepare command to read weight
    const std::vector<BYTE> data = { 0x40, 0x01, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 };
    std::vector<BYTE> requestData = data;
    requestData[1] = static_cast<BYTE>(canId & 0xFF);

    // Send the command to the CAN bus
    manualRead->SendMessages(requestData, canId);

    // Read the response from the CAN bus
    auto [status, responseID, rxCommand] = manualRead->ReadMessages();

    // Check if the response matches the expected format
    if (status == PCAN_ERROR_OK && rxCommand.size() >= 6 && rxCommand[1] == requestData[1])
    {
        // Extract and process weight data (signed value handling)
        int decimalValue = (rxCommand[7] << 24) | (rxCommand[6] << 16) | (rxCommand[5] << 8) | rxCommand[4];

        // Check if the sign bit (23rd bit) is set
        if (decimalValue & 0x800000)
        {
            decimalValue |= 0xFF000000; // Set upper bits to 1
        }

        // Convert to double and apply scaling factor
        double weightInDouble = static_cast<double>(decimalValue) / SCALING_FACTOR;

        // Set the weight in LoadCell, which will emit the signal
        // Example: auto loadCell = loadCellMap[canId];
        // if (loadCell) loadCell->setWeight(weightInDouble);
    }
}

void LoadCellManager::updateADCValue(int canId)
{
    // Command to read ADC value
    const std::vector<BYTE> data = { 0x40, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 };

    // Send the command via CAN bus
    manualRead->SendMessages(data, canId);

    // Read the response from CAN bus
    auto [status, responseID, rxCommand] = manualRead->ReadMessages();

    // Check if the response matches the expected format
    if (status == PCAN_ERROR_OK && rxCommand.size() >= 6 && rxCommand[1] == data[1] && rxCommand[2] == data[2])
    {
        // Extract the ADC value
        int decimalValue = (rxCommand[7] << 24) | (rxCommand[6] << 16) | (rxCommand[5] << 8) | rxCommand[4];

        // Set the ADC value in LoadCell, which will emit the signal
        // Example: auto loadCell = loadCellMap[canId];
        // if (loadCell) loadCell->setADC(decimalValue);
    }
}

void LoadCellManager::deleteLoadCell(int canId)
{
    // Remove the mapping for the load cell
    canIdToInternalIdMap.erase(canId);

    // Optionally, you could also delete the LoadCell instance if needed
    // Example: delete loadCellMap[canId];
    // loadCellMap.erase(canId);
}

void LoadCellManager::onTimerTimeout()
{
    updateAllLoadCells();  // Update weight and ADC values for all load cells
}
