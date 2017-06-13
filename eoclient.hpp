#ifndef EOCLIENT_HPP_INCLUDED
#define EOCLIENT_HPP_INCLUDED

#include "config.hpp"
#include "packet.hpp"
#include "handlers/handlers.hpp"
#include "character.hpp"

#include <SFML/Network.hpp>
#include <string>
#include <memory>

using std::shared_ptr;

class EOClient // this is an "eo connection specialist" - it will be mainly used for management of connection and packets
{
public:
    enum class PacketState
    {
        ReadLen1,
        ReadLen2,
        ReadData
    };

    enum class State
    {
        Uninitialized,
        Initialized,
        LoggedIn,
        Playing
    };

private:
    sf::TcpSocket socket;
    bool connected;

    std::string send_buffer;
    std::string recv_buffer;

    PacketState packet_state;
    unsigned char raw_length[2];
    unsigned int length;
    std::string data;

    typedef void(*handler_func)(PacketReader packet);
    std::map<PacketFamily, std::map<PacketAction, handler_func>> handlers;

    int seq_start;
    int seq;

    int session_id;

    State state;

public:
    PacketProcessor processor;
    std::vector<shared_ptr<Character>> account_characters;

    EOClient(bool initialize = false);
    bool Connect();
    void Disconnect();
    bool Connected();
    void Send(PacketBuilder packet);
    void HandleData(std::string data);
    void RegisterHandler(PacketFamily family, PacketAction action, handler_func func);
    void UnregisterHandler(PacketFamily family, PacketAction action);
    void Tick();
    void Reset();
    void InitSequenceByte(unsigned char s1, unsigned char s2);
    void UpdateSequenceByte(unsigned short s1, unsigned char s2);
    int GenSequenceByte();
    void SetState(State state);
    State GetState();

    void RequestInit();
    void Initialize(PacketReader reader);
    void LoginRequest(std::string username, std::string password);
    void AccountRequest(std::string username);
    void AccountCreate(std::string username, std::string password, std::string real_name, std::string location, std::string email);
    void RequestSelectCharacter(unsigned int id);

    shared_ptr<Character> GetAccountCharacter(std::size_t index);
};

#endif // EOCLIENT_HPP_INCLUDED
