#include <regex>

#include "RebelsCommunication.h"

// Receive the public key from the rebels and check if it's valid.
std::string RebelsCommunication::ReceivePublicKey(Connection& rebelsConnection)
{
    std::vector<unsigned char> message = rebelsConnection.ReceiveMessage();

    std::string publicKey(message.begin(),message.end());

    // The public key should be a string with two numbers inside it, separated by a space, in the
    // format "XXXX YYYY" to be valid.
    if (std::regex_search(publicKey, std::regex("")))
    {
        return publicKey;
    }
    else
    {
        throw std::runtime_error("Invalid public key received from the Rebels. (" + publicKey + ")");
    }
}