#include <iostream>
#include <string.h>

#include "EmpireMessage.h"

// Private methods

void EmpireMessage::ReceiveMessage(Connection& empireConnection)
{
    std::vector<unsigned char> sizeBytes = empireConnection.ReceiveMessageExactSize(2);
    payloadSize = (sizeBytes[0] << 8) + sizeBytes[1];

    do
    {
        messageRawData = empireConnection.ReceiveMessageExactSize(payloadSize - 1);
        messageChecksum = empireConnection.ReceiveMessageExactSize(1)[0];

        if (!isChecksumCorrect())
        {
            std::cout << "Receive a message from the Empire containing a invalid checksum, asking the server to send again." << std::endl;
            empireConnection.SendMessage("send again");
        }

    } while (!isChecksumCorrect());
}


// Returns true if the messages checksum is correct otherwise returns false.
bool EmpireMessage::isChecksumCorrect()
{
    uint8_t calculatedChecksum = 0;

    // Add the first two bytes, that represents the size, to the checksum
    calculatedChecksum += (uint8_t) payloadSize >> 8;
    calculatedChecksum += (uint8_t) payloadSize;

    for (auto character : messageRawData)
    {
        calculatedChecksum += character;
    }

    return calculatedChecksum == messageChecksum;
}


// Check if the message is a know plain ASCII message or is encrypted
void EmpireMessage::CheckMessageType()
{
    if (strncmp((char *) &messageRawData[0], "User accepted.", messageRawData.size()) == 0 ||
        strncmp((char *) &messageRawData[0], "Game over!", messageRawData.size()) == 0)
    {
        type = EmpireMessageType::ASCII;
    }
    else
    {
        type = EmpireMessageType::ENCRYPTED;
    }
}


// Public methods

EmpireMessage::EmpireMessage(Connection& empireConnection)
{
    ReceiveMessage(empireConnection);
    CheckMessageType();

    if (type == EmpireMessageType::ASCII)
    {
        messageData = std::string(messageRawData.begin(), messageRawData.end());
    }
    else // type == EmpireMessageType::ENCRYPTED
    {}
}


// Returns the message type received from the Empire.
EmpireMessageType EmpireMessage::GetType() const
{
    return type;
}


// Returns the decrypted message received from the Empire.
std::string EmpireMessage::GetMessageData() const
{
    return messageData;
}