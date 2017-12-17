#ifndef EMPIRE_MESSAGE_H
#define EMPIRE_MESSAGE_H

#include <string>
#include "Connection.h"

enum class EmpireMessageType {ASCII, ENCRYPTED};

class EmpireMessage
{
private:
    // Identify if the message is plain ASCII or encrypted.
    EmpireMessageType type;

    // Message payload size.
    uint16_t payloadSize;

    // Payload of the message containing the raw message data that may be encrypted and the
    // checksum.
    std::vector<unsigned char> messageRawData;
    uint8_t messageChecksum;

    // Decrypted message.
    std::string messageData;

    void ReceiveMessage(Connection& empireConnection);
    bool isChecksumCorrect();
    void CheckMessageType();

public:
    EmpireMessage(Connection& empireConnection);

    EmpireMessageType GetType() const;
    std::string GetMessageData() const;
};

#endif // EMPIRE_MESSAGE_H