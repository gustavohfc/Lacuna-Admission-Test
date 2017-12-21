#ifndef EMPIRE_MESSAGE_H
#define EMPIRE_MESSAGE_H

#include <string>
#include <exception>
#include "Connection.h"

// Types of messages sent by the Empire.
enum class EmpireMessageType {ASCII, ENCRYPTED};


// Define a new exeption class to indicate that the message from the empire couldn't be decrypted.
class DecryptEmpireMessageException : public std::exception
{
public:
    virtual const char* what() const throw()
    {
        return "Couldn't find the encryption key of a message from the Empire, ignoring this message.";
    }
};


class EmpireMessage
{
private:
    // Identify if the message is plain ASCII or encrypted.
    EmpireMessageType type;

    // Payload size, determined by the first two bytes of the message.
    uint16_t payloadSize;

    // Payload of the message containing the raw message data, that may be encrypted, and the checksum.
    std::vector<unsigned char> messageRawData;
    uint8_t messageChecksum;

    // Decrypted message.
    std::string messageData;

    void ReceiveMessage(Connection& empireConnection);
    bool IsChecksumCorrect();
    void CheckMessageType();
    void DecryptMessage();
    uint8_t GetEncrytKey();

public:
    EmpireMessage(Connection& empireConnection);

    EmpireMessageType GetType() const;
    std::string GetMessageData() const;
    bool ContainCoordinates();
};

#endif // EMPIRE_MESSAGE_H