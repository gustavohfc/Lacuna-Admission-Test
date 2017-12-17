#include <iostream>
#include <exception>
#include <string>
#include <unistd.h>
#include "Connection.h"
#include "EmpireMessage.h"

#include <stdio.h>


// Server address, it's the same for both the empire and the rebels
const std::string SERVER_ADDR = "lacuna.ddns.net";

// Empire Server Port
const std::string EMPIRE_PORT = "21816";

// Rebel's Server Port
const std::string REBEL_PORT = "31816";

// Authentication token, it's the same for connections with both the empire and the rebels
const std::string TOKEN = "a74cbe68-dfa5-446d-8868-da326a2baa62";

// Function prototypes
void HackTheEmpire(Connection& empireConnection);


int main()
{
    try
    {
        Connection empireConnection(SERVER_ADDR, EMPIRE_PORT);
        empireConnection.Authenticate(TOKEN);

        // Connection rebelsConnection();
        // rebelsConnection.Authenticate(TOKEN);
        // throw std::runtime_error("teste");

        HackTheEmpire(empireConnection);
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch ( ... )
    {
        std::cerr << "Error: Unexpected exception" << std::endl;
        throw; // Re-throw the unexpected exception
    }

    return 0;
}


// Get messages from the empire, decode the information and send to the rebels until receive a "sucess" message
void HackTheEmpire(Connection& empireConnection)
{
    // Check if the user Token was accepted by the Empire server
    EmpireMessage empireMessage(empireConnection);
    if (empireMessage.GetType() != EmpireMessageType::ASCII ||
        empireMessage.GetMessageData() != "User accepted.")
    {
        throw std::runtime_error("The user Token wasn't accepted by the Empire server.");
    }

    empireConnection.SendMessage("stop");
}