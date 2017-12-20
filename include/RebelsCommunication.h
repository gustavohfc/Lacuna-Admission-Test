#include <string>
#include "Connection.h"

// Namespace with auxiliary functions for communication with the Rebels
namespace RebelsCommunication
{
    std::string ReceivePublicKey(Connection& rebelsConnection);
    std::string ReceiveResponse(Connection& rebelsConnection);
    std::vector<unsigned char> MakeRebelsMessage(std::string messageData, std::string publicKey);
    unsigned char ModularExponentiationEncrypt(unsigned char base, uint encryptExponent, uint modulus);
}