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
    : connectionID(address + ':' + port)
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
        throw std::runtime_error(std::string("getaddrinfo: ") + gai_strerror(rv) + ". [ Connection ID: " + connectionID + " ]");        
    }

    for (servInfo = servInfoList; servInfo != nullptr; servInfo = servInfo->ai_next)
    {
        if ((sockfd = socket(servInfo->ai_family, servInfo->ai_socktype, 0)) < 0)
        {
            throw std::runtime_error(std::string("socket: ") + std::strerror(errno) + ". [ Connection ID: " + connectionID + " ]");
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
        throw std::runtime_error("Failed to connect to the server. [ Connection ID: " + connectionID + " ]");
    }

    freeaddrinfo(servInfoList);
}


// Closes the connection.
Connection::~Connection()
{
    close(sockfd);
}


// Send the authentication token to the server.
void Connection::Authenticate(std::string token)
{
    SendMessage(token);
}


// Convert the message string to a char vector so it can be send to the server.
void Connection::SendMessage(std::string message)
{
    SendMessage(std::vector<unsigned char> (message.begin(), message.end()));
}


// Send all the bytes in the vector message to the server, a std::vector<unsigned char> is used so the
// encrypted message can contain characters that would be invalid in a std::string for example
// the '\0'.
void Connection::SendMessage(std::vector<unsigned char> message)
{
    uint bytesSent = 0;

    while (bytesSent != message.size())
    {
        int rv = send(sockfd, &message[bytesSent], message.size() - bytesSent, 0);
        if (rv < 0)
        {
            throw std::runtime_error(std::string("send: ") + std::strerror(errno) + ". [ Connection ID: " + connectionID + " ]");
        }

        bytesSent += rv;
    }
}


// Receive a message with a exact size in bytes determined by the parameter messageSize.
std::vector<unsigned char> Connection::ReceiveMessageExactSize(unsigned messageSize)
{
    std::vector<unsigned char> message;

    // Receive more data until the message has the expected size.
    while (message.size() != messageSize)
    {
        std::vector<unsigned char> newData = ReceiveMessage(messageSize - message.size());

        // Append the newData in the end of the message.
        message.insert(message.end(), newData.begin(), newData.end());
    }

    return message;
}


std::vector<unsigned char> Connection::ReceiveMessage(unsigned maxBytes)
{
    std::vector<unsigned char> message;
    message.resize(maxBytes);

    int bytesReceived = recv(sockfd, &message[0], message.capacity(), 0);
    if (bytesReceived < 0)
    {
        throw std::runtime_error(std::string("recv: ") + std::strerror(errno) + ". [ Connection ID: " + connectionID + " ]");
    }
    else if (bytesReceived == 0)
    {
        throw std::runtime_error("recv: Connection close by the server. [ Connection ID: " + connectionID + " ]");
    }

    message.resize(bytesReceived);

    return message;
}