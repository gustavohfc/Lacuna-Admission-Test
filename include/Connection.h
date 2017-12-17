#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>
#include <vector>

class Connection
{
private:
    // Socket file descriptor
    int sockfd;

    // The connection ID is the 'SERVER_ADDRESS':'SERVER_PORT'
    std::string connectionID;

public:
    Connection(std::string address, std::string port);
    ~Connection();

    void Authenticate(std::string token);
    void SendMessage(std::string message);
    void SendMessage(std::vector<unsigned char> message);
    std::vector<unsigned char> ReceiveMessageExactSize(unsigned messageSize);
    std::vector<unsigned char> ReceiveMessage(unsigned maxBytes);
};

#endif // CONNECTION_H