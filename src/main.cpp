#include <iostream>
#include <exception>
#include <string>
#include <unistd.h>
#include "Connection.h"

// Server address, it's the same for both the empire and the rebels
const std::string SERVER_ADDR = "lacuna.ddns.net";

// Empire Server Port
const std::string EMPIRE_PORT = "21816";

// Rebel's Server Port
const std::string REBEL_PORT = "31816";

// Authentication token, it's the same for connections with both the empire and the rebels
const std::string TOKEN = "a74cbe68-dfa5-446d-8868-da326a2baa62";


int main()
{
    try
    {
        Connection empireConnection(SERVER_ADDR, EMPIRE_PORT);
        empireConnection.Authenticate(TOKEN);

        // Connection rebelsConnection();
        // rebelsConnection.Authenticate(TOKEN);
        // throw std::runtime_error("teste");

        usleep(500);
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch ( ... )
    {
        std::cerr << "Error: Unexpected exception" << std::endl;
        throw; // Re-throw the exception
    }

    return 0;
}