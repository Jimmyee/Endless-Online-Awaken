// Endless Online Awaken

#include "init.hpp"

#include "login.hpp"
#include "account.hpp"
#include "../client.hpp"
#include "../server.hpp"

#include <iostream>
#include <array>

namespace PacketHandlers::HInit
{

void Main(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
{
    Client *client = (Client *)data_ptr[0];

    if(client->state != Client::State::Uninitialized) return;

    std::array<int, 3> version = { 0, 0, 0 };

    for(int i = 0; i < 3; ++i)
    {
        packet >> version[i];
    }

    bool valid = true;

    for(int i = 0; i < 3; ++i)
    {
        if(version[i] != Server().GetClientVersion()[i]) valid = false;
    }

    unsigned char answer = 0;

    if(valid)
    {
        answer = 1;

        client->state = Client::State::Initialized;
        client->packet_handler.Register(PacketID::Login, PacketHandlers::HLogin::Main, data_ptr);
        client->packet_handler.Register(PacketID::Account, PacketHandlers::HAccount::Main, data_ptr);
    }
    else
    {
        answer = 2;
    }

    sf::Packet reply;
    reply << (unsigned short)PacketID::Init;
    reply << answer;
    client->Send(reply);

    std::cout << "Client initialized." << std::endl;
    std::cout << "Client state: " << (int)client->state << std::endl;
}

}
