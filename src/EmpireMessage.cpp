#include <iostream>
#include <regex>

#include "EmpireMessage.h"

// Private methods

// Receive a new message from the empire
void EmpireMessage::ReceiveMessage(Connection& empireConnection)
{
    std::vector<unsigned char> sizeBytes = empireConnection.ReceiveMessageExactSize(2);
    payloadSize = (sizeBytes[0] << 8) + sizeBytes[1];

    do
    {
        messageRawData = empireConnection.ReceiveMessageExactSize(payloadSize - 1);
        messageChecksum = empireConnection.ReceiveMessageExactSize(1)[0];

        // If the checksum is invilid then ask the server to send again.
        if (!IsChecksumCorrect())
        {
            std::cout << "Receive a message from the Empire containing a invalid checksum, asking the server to send again." << std::endl;
            empireConnection.SendMessage("send again");
        }

    } while (!IsChecksumCorrect());
}


// Returns true if the messages checksum is correct otherwise returns false.
bool EmpireMessage::IsChecksumCorrect()
{
    uint8_t calculatedChecksum = 0;

    // Add the first two bytes, that represents the size, to the checksum
    calculatedChecksum += (uint8_t) payloadSize >> 8;
    calculatedChecksum += (uint8_t) payloadSize;

    for (uint8_t byte : messageRawData)
    {
        calculatedChecksum += byte;
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


// Decrypted message and saves on messageData.
void EmpireMessage::DecryptMessage()
{
    uint8_t key = GetEncrytKey();

    for (auto byte : messageRawData)
    {
        messageData += byte ^ key;
    }
}


// Calculate the encrypt knowing that the messages from the Empire contain the string "Vader". If
// the encryption key couldn't be found it throws the exception DecryptEmpireMessageException.
uint8_t EmpireMessage::GetEncrytKey()
{
    // Move the mask "Vader" over the messageRawData trying to find the key
    for (unsigned i = 0; i < messageRawData.size() - 4; i++)
    {
        uint8_t possibleKey = messageRawData[i] ^ 'V';

        if (possibleKey == (messageRawData[i + 1] ^ 'a') &&
            possibleKey == (messageRawData[i + 2] ^ 'd') &&
            possibleKey == (messageRawData[i + 3] ^ 'e') &&
            possibleKey == (messageRawData[i + 4] ^ 'r') )
        {
            return possibleKey;
        }
    }

    throw DecryptEmpireMessageException();
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
    {
        DecryptMessage();
    }
}


// Returns the message type received from the Empire.
EmpireMessageType EmpireMessage::GetType() const
{
    return type;
}


// Returns the decrypted or ASCII message received from the Empire.
std::string EmpireMessage::GetMessageData() const
{
    return messageData;
}


// Check if there are coordinates in the message
bool EmpireMessage::ContainCoordinates()
{
    return std::regex_search(messageData, std::regex("(x)[[:digit:]]+(y)[[:digit:]]+"));
}