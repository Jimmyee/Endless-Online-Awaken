// Endless Online Awaken

#ifndef SERVER_HPP_INCLUDED
#define SERVER_HPP_INCLUDED

#include <SFML/Network.hpp>
#include <memory>

#include "client.hpp"

// class responsible for managing clients and server-client data transfer (monostate)
class Server
{
private:
    static bool initialized_;
    static sf::TcpListener listener;
    static std::vector<std::unique_ptr<Client>> clients;

public:
    Server();
    Server(unsigned short port);
    void Tick();
    Client *GetClient(std::string char_name);
};

#endif // SERVER_HPP_INCLUDED
