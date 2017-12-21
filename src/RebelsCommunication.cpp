#include <stdlib.h>
#include <regex>
#include <algorithm>

#include "RebelsCommunication.h"

// Receive the public key from the rebels and check if it's valid.
std::string RebelsCommunication::ReceivePublicKey(Connection& rebelsConnection)
{
    std::vector<unsigned char> message = rebelsConnection.ReceiveMessage();

    std::string publicKey(message.begin(),message.end());

    // The public key should be a string with two numbers inside it, separated by a space, in the
    // format "XXXX YYYY" to be valid.
    if (std::regex_match(publicKey, std::regex("[[:digit:]]+( )[[:digit:]]+")))
    {
        return publicKey;
    }
    else
    {
        throw std::runtime_error("Invalid public key received from the Rebels. (" + publicKey + ")");
    }
}


// Receive the ASCII stream response from the Rebels.
std::string RebelsCommunication::ReceiveResponse(Connection& rebelsConnection)
{
    std::vector<unsigned char> response = rebelsConnection.ReceiveMessage();

    // Converts the response to a string as it has only ASCII characters
    return std::string(response.begin(), response.end());
}


// Returns a encrypted message to be send to the Rebels.
std::vector<unsigned char> RebelsCommunication::MakeRebelsMessage(std::string messageData, std::string publicKey)
{
    std::vector<unsigned char> message;

    message.push_back((unsigned char) messageData.size() >> 8); // Most significant byte from the size
    message.push_back((unsigned char) messageData.size()); // Least significant byte from the size

    // Encrypt the message
    uint encryptExponent = atoi(publicKey.c_str());
    uint modulus = atoi(publicKey.substr(publicKey.find(" ") + 1).c_str());

    for (auto letter : messageData)
    {
        message.push_back(ModularExponentiationEncrypt(letter, encryptExponent, modulus));
    }

    return message;
}


// Calculate the modular exponentiation encryption for the base
unsigned char RebelsCommunication::ModularExponentiationEncrypt(unsigned char base, uint encryptExponent, uint modulus)
{
    uint result = 1;

    for (uint exponent = 1; exponent <= encryptExponent; exponent++)
    {
        result = (result * base) % modulus;
    }

    return result;
}
