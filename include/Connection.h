#include <string>
#include <vector>

#ifndef CONNECTION_H
#define CONNECTION_H

class Connection
{
private:
    // Socket file descriptor
    int sockfd;

public:
    Connection(std::string address, std::string port);
    ~Connection();

    void Authenticate(std::string token);
    void sendMessage(std::string message);
    void sendMessage(std::vector<char> message);
};

#endif // CONNECTION_H