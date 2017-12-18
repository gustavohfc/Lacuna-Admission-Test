#include <string>
#include "Connection.h"

// Namespace with functions assist the communication with the rebels
namespace RebelsCommunication
{
    std::string ReceivePublicKey(Connection& rebelsConnection);
}