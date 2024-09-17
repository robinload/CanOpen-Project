#include "utility.h"
#include <vector>
#include <iomanip>
#include <thread>  // For std::this_thread::sleep_for
#include <chrono>  // For std::chrono::milliseconds
#include <qDebug>


ManualRead::ManualRead()
{

    // ShowConfigurationHelp(); // Shows information about this sample
    // ShowCurrentConfiguration(); // Shows the current parameters configuration
}

ManualRead::~ManualRead()
{
    CAN_Uninitialize(PCAN_NONEBUS);
}



void ManualRead::SendMessages(const std::vector<BYTE>& data, uint32_t canId)
{

    TPCANStatus stsResult;
    TPCANMsg CANMsg;
    uint32_t modifiedCanId = (0x06 << 8) | (canId & 0xFF); // Assumes canId is 8-bit

    CANMsg.ID = modifiedCanId; // Example ID
    CANMsg.MSGTYPE = PCAN_MESSAGE_STANDARD;
    CANMsg.LEN = static_cast<BYTE>(data.size());

    // std::stringstream ss;
    // ss << "Data: ";
    // for (BYTE byte : data)
    // {
    //     ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
    // }

    // qDebug().noquote() << QString::fromStdString(ss.str()).trimmed();

    if (data.size() > sizeof(CANMsg.DATA))
    {
        std::cerr << "Data size exceeds CAN message data length.\n";
        return;
    }

    std::copy(data.begin(), data.end(), CANMsg.DATA);

    if (IsFD)
    {
        // stsResult = CAN_WriteFD(PcanHandle, &CANMsg); // Uncomment if using CAN FD
        return;
    }
    else
    {
        stsResult = CAN_Write(PcanHandle, &CANMsg); // Use CAN_Write for standard CAN
    }

    if (stsResult != PCAN_ERROR_OK)
    {
        ShowStatus(stsResult);
        return;
    }

    //for (BYTE byte : data) {
    //    // Print each byte in uppercase hexadecimal format, padded with leading zeros
    //    std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
    //}

    //std::cout << std::endl;
}

//void ManualRead::ReadMessages()
//{
//    TPCANStatus stsResult;
//
//    // We read at least one time the queue looking for messages. If a message is found, we look again trying to
//    // find more. If the queue is empty or an error occurr, we get out from the dowhile statement.
//    do
//    {
//        stsResult = IsFD ? ReadMessageFD() : ReadMessage();
//        std::cout << "Status: 0x" << std::hex << std::setw(8) << std::setfill('0') << stsResult << std::endl;
//        if (stsResult != PCAN_ERROR_OK && stsResult != PCAN_ERROR_QRCVEMPTY)
//        {
//            ShowStatus(stsResult);
//            return;
//        }
//    } while (!(stsResult & PCAN_ERROR_QRCVEMPTY));
//}


// std::pair<TPCANStatus, std::vector<BYTE>> ManualRead::ReadMessages()
// {
//     TPCANStatus stsResult;
//     std::vector<BYTE> allData;

//     do
//     {
//         // Read a single message and its data
//         auto [status, id, data] = ReadMessage();
//         // TPCANStatus status = result.first;
//         // std::vector<BYTE> data = result.second;
//     //         stsResult = status;
//     //         canId = id; // Store the CAN ID
//         stsResult = status;

//         //std::cout << "Status: 0x" << std::hex << std::setw(8) << std::setfill('0') << stsResult << std::endl;

//         if (stsResult != PCAN_ERROR_OK && stsResult != PCAN_ERROR_QRCVEMPTY)
//         {
//             ShowStatus(stsResult);
//             return { stsResult, allData }; // Return the error status and collected data
//         }

//         // Append received data to the allData vector
//         allData.insert(allData.end(), data.begin(), data.end());

//     } while (!(stsResult & PCAN_ERROR_QRCVEMPTY));

//     return { stsResult, allData }; // Return the status and all collected data
// }

std::tuple<TPCANStatus, UINT32, std::vector<BYTE>> ManualRead::ReadMessages()
{
    TPCANStatus stsResult;
    std::vector<BYTE> allData;
    UINT32 canId = 0;

    do
    {
        // Read a single message and its components
        auto [status, id, data] = ReadMessage();
        stsResult = status;
        canId = id; // Store the CAN ID

        // Log the CAN ID for debugging in hexadecimal format
        qDebug() << "CAN ID:" << QString("0x%1").arg(canId, 0, 16).toUpper();

        if (stsResult != PCAN_ERROR_OK && stsResult != PCAN_ERROR_QRCVEMPTY)
        {
            ShowStatus(stsResult);
            return { stsResult, canId, allData }; // Return the error status, CAN ID, and collected data
        }

        // Append received data to the allData vector
        allData.insert(allData.end(), data.begin(), data.end());

    } while (!(stsResult & PCAN_ERROR_QRCVEMPTY));

    return { stsResult, canId, allData }; // Return the status, CAN ID, and all collected data
}

TPCANStatus ManualRead::ReadMessageFD()
{
    TPCANMsgFD CANMsg;
    TPCANTimestampFD CANTimeStamp;
    std::vector<BYTE> data;

    // We execute the "Read" function of the PCANBasic
    TPCANStatus stsResult = CAN_ReadFD(PcanHandle, &CANMsg, &CANTimeStamp);
    if (stsResult != PCAN_ERROR_QRCVEMPTY)
        // We process the received message
        ProcessMessageCanFD(CANMsg, CANTimeStamp);

    return stsResult;
}

// std::pair<TPCANStatus, std::vector<BYTE>> ManualRead::ReadMessage()
// {
//     TPCANMsg CANMsg;
//     TPCANTimestamp CANTimeStamp;
//     std::vector<BYTE> data;

//     // Execute the "Read" function of the PCANBasic
//     TPCANStatus stsResult = CAN_Read(PcanHandle, &CANMsg, &CANTimeStamp);

//     if (stsResult != PCAN_ERROR_QRCVEMPTY)
//     {
//         // Process the received message and store the data
//         data = ProcessMessageCan(CANMsg, CANTimeStamp);
//     }

//     // Return both the status and the data as a pair
//     return std::make_pair(stsResult, data);
// }

std::tuple<TPCANStatus, UINT32, std::vector<BYTE>> ManualRead::ReadMessage()
{
    TPCANMsg CANMsg;
    TPCANTimestamp CANTimeStamp;

    // Execute the "Read" function of the PCANBasic
    TPCANStatus stsResult = CAN_Read(PcanHandle, &CANMsg, &CANTimeStamp);

    if (stsResult != PCAN_ERROR_QRCVEMPTY)
    {
        // Process the received message and store the data
        auto processed = ProcessMessageCan(CANMsg, CANTimeStamp);
        UINT32 canId = processed.first;
        std::vector<BYTE> data = processed.second;
        // Return the status, CAN ID, and data
        return std::make_tuple(stsResult, canId, data);
    }

    // Return status, default CAN ID, and empty data
    return std::make_tuple(stsResult, 0, std::vector<BYTE>{});
}

//void ManualRead::ProcessMessageCan(TPCANMsg msg, TPCANTimestamp itsTimeStamp)
//{
//    UINT64 microsTimestamp = itsTimeStamp.micros + (1000ULL * itsTimeStamp.millis) + (0x100'000'000ULL * 1000ULL * itsTimeStamp.millis_overflow);
//
//    std::cout << "Type: " << GetMsgTypeString(msg.MSGTYPE) << "\n";
//    std::cout << "ID: " << GetIdString(msg.ID, msg.MSGTYPE) << "\n";
//    char result[MAX_PATH] = { 0 };
//    sprintf_s(result, sizeof(result), "%i", msg.LEN);
//    std::cout << "Length: " << result << "\n";
//    std::cout << "Time: " << GetTimeString(microsTimestamp) << "\n";
//    std::cout << "Data: " << GetDataString(msg.DATA, msg.MSGTYPE, msg.LEN) << "\n";
//    std::cout << "----------------------------------------------------------\n";
//}

// std::vector<BYTE> ManualRead::ProcessMessageCan(TPCANMsg msg, TPCANTimestamp itsTimeStamp)
// {
//     UINT64 microsTimestamp = itsTimeStamp.micros + (1000ULL * itsTimeStamp.millis) + (0x100'000'000ULL * 1000ULL * itsTimeStamp.millis_overflow);

//     //std::cout << "Type: " << GetMsgTypeString(msg.MSGTYPE) << "\n";
//     // std::cout << "ID: " << GetIdString(msg.ID, msg.MSGTYPE) << "\n";

//     char result[MAX_PATH] = { 0 };
//     sprintf_s(result, sizeof(result), "%i", msg.LEN);
//     //std::cout << "Length: " << result << "\n";

//     //std::cout << "Time: " << GetTimeString(microsTimestamp) << "\n";

//     // Create a vector from the DATA array
//     std::vector<BYTE> dataVector(msg.DATA, msg.DATA + msg.LEN);
//     /*std::cout << "Data: ";*/
//     //for (BYTE byte : dataVector) {
//     //    std::cout << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << static_cast<int>(byte) << " ";
//     //}
//     //std::cout << "\n";

//     //std::cout << "----------------------------------------------------------\n";

//     // Return the vector
//     return dataVector;
// }

std::pair<UINT32, std::vector<BYTE>> ManualRead::ProcessMessageCan(TPCANMsg msg, TPCANTimestamp itsTimeStamp)
{
    UINT64 microsTimestamp = itsTimeStamp.micros + (1000ULL * itsTimeStamp.millis) + (0x100'000'000ULL * 1000ULL * itsTimeStamp.millis_overflow);

    // Create a vector from the DATA array
    std::vector<BYTE> dataVector(msg.DATA, msg.DATA + msg.LEN);

    // Return the CAN ID and the data vector as a pair
    return std::make_pair(msg.ID, dataVector);
}



void ManualRead::ProcessMessageCanFD(TPCANMsgFD msg, TPCANTimestampFD itsTimeStamp)
{
    std::cout << "Type: " << GetMsgTypeString(msg.MSGTYPE) << "\n";
    std::cout << "ID: " << GetIdString(msg.ID, msg.MSGTYPE) << "\n";
    std::cout << "Length: " << GetLengthFromDLC(msg.DLC) << "\n";
    std::cout << "Time: " << GetTimeString(itsTimeStamp) << "\n";
    std::cout << "Data: " << GetDataString(msg.DATA, msg.MSGTYPE, GetLengthFromDLC(msg.DLC)) << "\n";
    std::cout << "----------------------------------------------------------\n";
}

void ManualRead::ShowConfigurationHelp()
{
    std::cout << "=========================================================================================\n";
    std::cout << "|                           PCAN-Basic ManualRead Example                                |\n";
    std::cout << "=========================================================================================\n";
    std::cout << "Following parameters are to be adjusted before launching, according to the hardware used |\n";
    std::cout << "                                                                                         |\n";
    std::cout << "* PcanHandle: Numeric value that represents the handle of the PCAN-Basic channel to use. |\n";
    std::cout << "              See 'PCAN-Handle Definitions' within the documentation                     |\n";
    std::cout << "* IsFD: Boolean value that indicates the communication mode, CAN (false) or CAN-FD (true)|\n";
    std::cout << "* Bitrate: Numeric value that represents the BTR0/BR1 bitrate value to be used for CAN   |\n";
    std::cout << "           communication                                                                 |\n";
    std::cout << "* BitrateFD: String value that represents the nominal/data bitrate value to be used for  |\n";
    std::cout << "             CAN-FD communication                                                        |\n";
    std::cout << "=========================================================================================\n";
    std::cout << "\n";
}

void ManualRead::ShowCurrentConfiguration()
{
    std::cout << "Parameter values used\n";
    std::cout << "----------------------\n";
    char buffer[MAX_PATH];
    FormatChannelName(PcanHandle, buffer, IsFD);
    std::cout << "* PCANHandle: " << buffer << "\n";
    if (IsFD)
        std::cout << "* IsFD: True\n";
    else
        std::cout << "* IsFD: False\n";
    ConvertBitrateToString(Bitrate, buffer);
    std::cout << "* Bitrate: " << buffer << "\n";
    std::cout << "* BitrateFD: " << BitrateFD << "\n";
    std::cout << "\n";
}

void ManualRead::ShowStatus(TPCANStatus status)
{
    std::cout << "In Show Status\n";
    std::cout << "=========================================================================================\n";
    char buffer[MAX_PATH];
    GetFormattedError(status, buffer);
    std::cout << buffer << "\n";
    std::cout << "=========================================================================================\n";
}

void ManualRead::FormatChannelName(TPCANHandle handle, LPSTR buffer, bool isFD)
{
    TPCANDevice devDevice;
    BYTE byChannel;

    // Gets the owner device and channel for a PCAN-Basic handle
    if (handle < 0x100)
    {
        devDevice = (TPCANDevice)(handle >> 4);
        byChannel = (BYTE)(handle & 0xF);
    }
    else
    {
        devDevice = (TPCANDevice)(handle >> 8);
        byChannel = (BYTE)(handle & 0xFF);
    }

    // Constructs the PCAN-Basic Channel name and return it
    char handleBuffer[MAX_PATH];
    GetTPCANHandleName(handle, handleBuffer);
    if (isFD)
        sprintf_s(buffer, MAX_PATH, "%s:FD %d (%Xh)", handleBuffer, byChannel, handle);
    else
        sprintf_s(buffer, MAX_PATH, "%s %d (%Xh)", handleBuffer, byChannel, handle);
}

void ManualRead::GetTPCANHandleName(TPCANHandle handle, LPSTR buffer)
{
    strcpy_s(buffer, MAX_PATH, "PCAN_NONE");
    switch (handle)
    {
    case PCAN_PCIBUS1:
    case PCAN_PCIBUS2:
    case PCAN_PCIBUS3:
    case PCAN_PCIBUS4:
    case PCAN_PCIBUS5:
    case PCAN_PCIBUS6:
    case PCAN_PCIBUS7:
    case PCAN_PCIBUS8:
    case PCAN_PCIBUS9:
    case PCAN_PCIBUS10:
    case PCAN_PCIBUS11:
    case PCAN_PCIBUS12:
    case PCAN_PCIBUS13:
    case PCAN_PCIBUS14:
    case PCAN_PCIBUS15:
    case PCAN_PCIBUS16:
        strcpy_s(buffer, MAX_PATH, "PCAN_PCI");
        break;

    case PCAN_USBBUS1:
    case PCAN_USBBUS2:
    case PCAN_USBBUS3:
    case PCAN_USBBUS4:
    case PCAN_USBBUS5:
    case PCAN_USBBUS6:
    case PCAN_USBBUS7:
    case PCAN_USBBUS8:
    case PCAN_USBBUS9:
    case PCAN_USBBUS10:
    case PCAN_USBBUS11:
    case PCAN_USBBUS12:
    case PCAN_USBBUS13:
    case PCAN_USBBUS14:
    case PCAN_USBBUS15:
    case PCAN_USBBUS16:
        strcpy_s(buffer, MAX_PATH, "PCAN_USB");
        break;

    case PCAN_LANBUS1:
    case PCAN_LANBUS2:
    case PCAN_LANBUS3:
    case PCAN_LANBUS4:
    case PCAN_LANBUS5:
    case PCAN_LANBUS6:
    case PCAN_LANBUS7:
    case PCAN_LANBUS8:
    case PCAN_LANBUS9:
    case PCAN_LANBUS10:
    case PCAN_LANBUS11:
    case PCAN_LANBUS12:
    case PCAN_LANBUS13:
    case PCAN_LANBUS14:
    case PCAN_LANBUS15:
    case PCAN_LANBUS16:
        strcpy_s(buffer, MAX_PATH, "PCAN_LAN");
        break;

    default:
        strcpy_s(buffer, MAX_PATH, "UNKNOWN");
        break;
    }
}

void ManualRead::GetFormattedError(TPCANStatus error, LPSTR buffer)
{
    // Gets the text using the GetErrorText API function. If the function success, the translated error is returned.
    // If it fails, a text describing the current error is returned.
    if (CAN_GetErrorText(error, 0x09, buffer) != PCAN_ERROR_OK)
        sprintf_s(buffer, MAX_PATH, "An error occurred. Error-code's text (%Xh) couldn't be retrieved", error);
}

void ManualRead::ConvertBitrateToString(TPCANBaudrate bitrate, LPSTR buffer)
{
    switch (bitrate)
    {
    case PCAN_BAUD_1M:
        strcpy_s(buffer, MAX_PATH, "1 MBit/sec");
        break;
    case PCAN_BAUD_800K:
        strcpy_s(buffer, MAX_PATH, "800 kBit/sec");
        break;
    case PCAN_BAUD_500K:
        strcpy_s(buffer, MAX_PATH, "500 kBit/sec");
        break;
    case PCAN_BAUD_250K:
        strcpy_s(buffer, MAX_PATH, "250 kBit/sec");
        break;
    case PCAN_BAUD_125K:
        strcpy_s(buffer, MAX_PATH, "125 kBit/sec");
        break;
    case PCAN_BAUD_100K:
        strcpy_s(buffer, MAX_PATH, "100 kBit/sec");
        break;
    case PCAN_BAUD_95K:
        strcpy_s(buffer, MAX_PATH, "95,238 kBit/sec");
        break;
    case PCAN_BAUD_83K:
        strcpy_s(buffer, MAX_PATH, "83,333 kBit/sec");
        break;
    case PCAN_BAUD_50K:
        strcpy_s(buffer, MAX_PATH, "50 kBit/sec");
        break;
    case PCAN_BAUD_47K:
        strcpy_s(buffer, MAX_PATH, "47,619 kBit/sec");
        break;
    case PCAN_BAUD_33K:
        strcpy_s(buffer, MAX_PATH, "33,333 kBit/sec");
        break;
    case PCAN_BAUD_20K:
        strcpy_s(buffer, MAX_PATH, "20 kBit/sec");
        break;
    case PCAN_BAUD_10K:
        strcpy_s(buffer, MAX_PATH, "10 kBit/sec");
        break;
    case PCAN_BAUD_5K:
        strcpy_s(buffer, MAX_PATH, "5 kBit/sec");
        break;
    default:
        strcpy_s(buffer, MAX_PATH, "Unknown Bitrate");
        break;
    }
}

std::string ManualRead::GetMsgTypeString(TPCANMessageType msgType)
{
    if ((msgType & PCAN_MESSAGE_STATUS) == PCAN_MESSAGE_STATUS)
        return "STATUS";

    if ((msgType & PCAN_MESSAGE_ERRFRAME) == PCAN_MESSAGE_ERRFRAME)
        return "ERROR";

    std::string strTemp;
    if ((msgType & PCAN_MESSAGE_EXTENDED) == PCAN_MESSAGE_EXTENDED)
        strTemp = "EXT";
    else
        strTemp = "STD";

    if ((msgType & PCAN_MESSAGE_RTR) == PCAN_MESSAGE_RTR)
        strTemp = (strTemp + "/RTR");
    else
        if (msgType > PCAN_MESSAGE_EXTENDED)
        {
            strTemp = (strTemp + " [ ");
            if (msgType & PCAN_MESSAGE_FD)
                strTemp = (strTemp + " FD");
            if (msgType & PCAN_MESSAGE_BRS)
                strTemp = (strTemp + " BRS");
            if (msgType & PCAN_MESSAGE_ESI)
                strTemp = (strTemp + " ESI");
            strTemp = (strTemp + " ]");
        }

    return strTemp;
}

std::string ManualRead::GetIdString(UINT32 id, TPCANMessageType msgType)
{
    char result[MAX_PATH] = { 0 };
    if ((msgType & PCAN_MESSAGE_EXTENDED) == PCAN_MESSAGE_EXTENDED)
    {
        sprintf_s(result, sizeof(result), "%08Xh", id);
        return result;
    }
    sprintf_s(result, sizeof(result), "%03Xh", id);
    return result;
}

int ManualRead::GetLengthFromDLC(BYTE dlc)
{
    switch (dlc)
    {
    case 9: return 12;
    case 10: return 16;
    case 11: return 20;
    case 12: return 24;
    case 13: return 32;
    case 14: return 48;
    case 15: return 64;
    default: return dlc;
    }
}

std::string ManualRead::GetTimeString(TPCANTimestampFD time)
{
    char result[MAX_PATH] = { 0 };
    double fTime = (time / 1000.0);
    sprintf_s(result, sizeof(result), "%.1f", fTime);
    return result;
}

std::string ManualRead::GetDataString(BYTE data[], TPCANMessageType msgType, int dataLength)
{
    if ((msgType & PCAN_MESSAGE_RTR) == PCAN_MESSAGE_RTR)
        return "Remote Request";
    else
    {
        char strTemp[MAX_PATH] = { 0 };
        std::string result = "";
        for (int i = 0; i < dataLength; i++)
        {
            sprintf_s(strTemp, sizeof(strTemp), "%02X ", data[i]);
            result.append(strTemp);
        }

        return result;
    }
}
