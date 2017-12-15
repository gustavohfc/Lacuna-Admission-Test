#include <iostream>
#include <exception>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include "Connection.h"

// Establishes a TCP connection with the server specified by the address and port parameters.
Connection::Connection(std::string address, std::string port)
{
    struct addrinfo hints, *servInfo, *servInfoList;
    int rv;

    // Set the socket to TCP and to accept both IPv4 and IPv6
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // Get the server IP address
    if ((rv = getaddrinfo(address.c_str(), port.c_str(), &hints, &servInfoList)) != 0)
    {
        throw std::runtime_error(std::string("getaddrinfo: ") + gai_strerror(rv));        
    }

    for (servInfo = servInfoList; servInfo != nullptr; servInfo = servInfo->ai_next)
    {
        if ((sockfd = socket(servInfo->ai_family, servInfo->ai_socktype, 0)) < 0)
        {
            throw std::runtime_error(std::string("socket: ") + std::strerror(errno));
        }

        // Try to connect to the server
        if (connect(sockfd, servInfo->ai_addr, servInfo->ai_addrlen) == -1)
        {
            close(sockfd);
            continue;
        }

        break;
    }

    if (servInfo == nullptr)
    {
        throw std::runtime_error("Failed to connect to the server.");
    }

    freeaddrinfo(servInfoList);
}


Connection::~Connection()
{
    close(sockfd);
}


// Send the authentication token to the server
void Connection::Authenticate(std::string token)
{
    sendMessage(token);
}


// Convert the message string to a char vector so it can be send to the server.
void Connection::sendMessage(std::string message)
{
    sendMessage(std::vector<char> (message.begin(), message.end()));
}


// Send all the bytes in the vector message to the server, a std::vector<char> is used so the
// encrypted message can contain characters that would be invalid in a std::string for example
// the '\0'.
void Connection::sendMessage(std::vector<char> message)
{
    uint bytesSent = 0;

    while (bytesSent != message.size())
    {
        int rv = send(sockfd, &message[bytesSent], message.size() - bytesSent, 0);
        if (rv < 0)
        {
            throw std::runtime_error(std::string("send: ") + std::strerror(errno));
        }

        bytesSent += rv;
    }
}