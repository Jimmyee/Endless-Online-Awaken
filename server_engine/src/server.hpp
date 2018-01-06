// Endless Online Awaken

#ifndef SERVER_HPP_INCLUDED
#define SERVER_HPP_INCLUDED

#include <SFML/Network.hpp>
#include <memory>
#include <array>

#include "client.hpp"

// class responsible for managing clients and server-client data transfer (monostate)
class Server
{
private:
    static bool initialized_;
    static sf::TcpListener listener;
    static std::vector<std::shared_ptr<sf::TcpSocket>> sockets;
    static std::vector<std::shared_ptr<Client>> clients;
    static std::array<int, 3> client_version;

public:
    Server();
    Server(unsigned short port);
    void Tick();
    Client *GetClientByChar(std::string char_name);
    Client *GetClientByAcc(std::string acc_name);
    std::array<int, 3> GetClientVersion();
};

#endif // SERVER_HPP_INCLUDED
