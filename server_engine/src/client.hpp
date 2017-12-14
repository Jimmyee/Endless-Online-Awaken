// Endless Online Awaken

#ifndef CLIENT_HPP_INCLUDED
#define CLIENT_HPP_INCLUDED

#include "packet_handler.hpp"
#include "character.hpp"

#include <SFML/Network.hpp>
#include <memory>

// for managing client's connection and data transfer
class Client
{
public:
    enum class State
    {
        Uninitialized,
        Initialized,
        Logged_in,
        Playing
    };

private:
    bool connected;
    std::vector<sf::Packet> recv_queue;
    std::vector<sf::Packet> send_queue;

public:
    State state;
    sf::TcpSocket socket;
    PacketHandler packet_handler;

    std::string username;
    std::vector<Character> characters;
    Character *character;

    Client();
    void Tick();
    bool Connected();
    void Send(sf::Packet &packet);
    sf::Packet Recv();

    Character *GetCharacter(std::string name);
    void RemoveCharacter(std::string name);
};

#endif // CLIENT_HPP_INCLUDED
