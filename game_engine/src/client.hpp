// Endless Online Awaken

#ifndef CLIENT_HPP_INCLUDED
#define CLIENT_HPP_INCLUDED

#include "packet_handler.hpp"
#include "const/entity.hpp"
#include "character.hpp"

#include <SFML/Network.hpp>
#include <memory>
#include <array>

// for managing client's connection and data transfer (monostate)
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
    static bool initialized_;
    static bool connected;
    static std::vector<sf::Packet> recv_queue;
    static std::vector<sf::Packet> send_queue;

public:
    static std::unique_ptr<sf::TcpSocket> socket;
    static PacketHandler packet_handler;
    static State state;

    static std::vector<std::shared_ptr<Character>> characters; // account characters
    static Character *character;

    static std::array<int, 3> version;

    Client();
    bool Connect(std::string address, unsigned short port);
    void Disconnect();
    void Tick();
    bool Connected();
    void Send(sf::Packet &packet);
    sf::Packet Recv();

    Character *GetCharacter(std::string name);
    void RemoveCharacter(std::string name);

    void Init();
    void Login(std::string username, std::string password);
    void CreateAccount(std::array<std::string, 5> input_data);
    void CreateCharacter(std::string name, Gender gender);
    void DeleteCharacter(std::string name);
    void CharacterList();
    void SelectCharacter(std::string name);
    void Talk(unsigned char channel, std::string message, std::string char_name = "");
    void Face(Direction direction);
    void Walk(Direction direction);
    void GetInRange(std::string name);
};

#endif // CLIENT_HPP_INCLUDED
