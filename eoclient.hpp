#ifndef EOCLIENT_HPP_INCLUDED
#define EOCLIENT_HPP_INCLUDED

#include <SFML/Network.hpp>

#include <string>

#include "config.hpp"
#include "packet.hpp"
#include "handlers/handlers.hpp"

class EOClient // this is an "eo connection specialist" - it will be mainly used for management of connection and packets
{
public:
    enum PacketState
    {
        ReadLen1,
        ReadLen2,
        ReadData
    };

    enum ClientState
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

    ClientState state;

public:
    PacketProcessor processor;

    EOClient(bool initialize = false);
    bool Connect();
    void Disconnect();
    void Send(PacketBuilder packet);
    void HandleData(std::string data);
    void RegisterHandler(PacketFamily family, PacketAction action, handler_func func);
    void UnregisterHandler(PacketFamily family, PacketAction action);
    void Tick();
    void Reset();
    void InitSequenceByte(unsigned char s1, unsigned char s2);
    void UpdateSequenceByte(unsigned short s1, unsigned char s2);
    int GenSequenceByte();
    ClientState GetState();

    void RequestInit();
    void Initialize(PacketReader reader);
};

#endif // EOCLIENT_HPP_INCLUDED
