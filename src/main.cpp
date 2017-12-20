#include <iostream>
#include <exception>
#include <string>
#include <unistd.h>
#include "Connection.h"
#include "EmpireMessage.h"
#include "RebelsCommunication.h"

// Server address, it's the same for both the empire and the rebels
const std::string SERVER_ADDR = "lacuna.ddns.net";

// Empire Server Port
const std::string EMPIRE_PORT = "21816";

// Rebel's Server Port
const std::string REBEL_PORT = "31816";

// Authentication token, it's the same for connections with both the empire and the rebels
const std::string TOKEN = "a74cbe68-dfa5-446d-8868-da326a2baa62";

// Function prototypes
void HackTheEmpire(Connection& empireConnection, Connection& rebelsConnection);



int main()
{
    try
    {
        Connection empireConnection(SERVER_ADDR, EMPIRE_PORT);
        empireConnection.Authenticate(TOKEN);

        Connection rebelsConnection(SERVER_ADDR, REBEL_PORT);
        rebelsConnection.Authenticate(TOKEN);

        HackTheEmpire(empireConnection, rebelsConnection);
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
void HackTheEmpire(Connection& empireConnection, Connection& rebelsConnection)
{
    std::string rebelsResponse;

    // Check if the user Token was accepted by the Empire server
    EmpireMessage empireMessage(empireConnection);
    if (empireMessage.GetType() != EmpireMessageType::ASCII || empireMessage.GetMessageData() != "User accepted.")
    {
        throw std::runtime_error("The user Token wasn't accepted by the Empire server.");
    }

    // Receive the public key from the Rebels
    std::string rebelsPublicKey = RebelsCommunication::ReceivePublicKey(rebelsConnection);

    do
    {
        empireConnection.SendMessage("tell me more");

        // Receive the response from the empire, if it's fail to decrypt the message then it's ignored
        try
        {
            std::cout << "\n\nReceiving new message from the Empire" << std::endl;
            empireMessage = EmpireMessage(empireConnection);
        }
        catch (DecryptEmpireMessageException& e)
        {
            // Fail decrypting the message, ignoring this message and going to the next.
            std::cout << e.what() << std::endl;
            continue;
        }

        if (empireMessage.GetType() == EmpireMessageType::ASCII && empireMessage.GetMessageData() == "Game over.")
        {
            throw std::runtime_error("Receive \"Game over!\" from the Empire server.");
        }

        std::cout << "Message received from the Empire: " << empireMessage.GetMessageData() << std::endl;

        // Check if there are coordinates in the message to be send to the Rebels
        if (empireMessage.ContainCoordinates())
        {
            std::cout << "The message contains coordinates, sending to the Rebels." << std::endl;

            // Encrypt the message with coordinates and send to the Rebels
            rebelsConnection.SendMessage(RebelsCommunication::MakeRebelsMessage(empireMessage.GetMessageData(), rebelsPublicKey));

            rebelsResponse = RebelsCommunication::ReceiveResponse(rebelsConnection);
            std::cout << "Message received from the Rebels: " << rebelsResponse << std::endl;
        }
        else
        {
            std::cout << "The message doesn't contain coordinates." << std::endl;
        }


    } while (rebelsResponse == "OK");

    if (rebelsResponse == "Success")
    {
        std::cout << "Empire information was successfully stolen and sent to the rebels." << std::endl;
    }
    else if (rebelsResponse == "Game over!")
    {
        std::cout << "\"Game over!\" received from the Rebels server, shutting down the execution." << std::endl;
    }

    empireConnection.SendMessage("stop");
}