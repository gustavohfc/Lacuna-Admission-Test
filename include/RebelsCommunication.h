#include <string>
#include "Connection.h"

// Namespace with functions assist the communication with the rebels
namespace RebelsCommunication
{
    std::string ReceivePublicKey(Connection& rebelsConnection);
    // std::string ReceiveResponse(Connection& rebelsConnection);
    std::vector<unsigned char> MakeRebelsMessage(std::string messageData, std::string publicKey);
}