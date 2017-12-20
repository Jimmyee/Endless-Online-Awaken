// Endless Online Awaken

#include "init.hpp"

#include "login.hpp"
#include "account.hpp"
#include "../client.hpp"

#include <iostream>

namespace PacketHandlers::HInit
{

void Main(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
{
    Client *client = (Client *)data_ptr[0];

    if(client->state != Client::State::Uninitialized) return;

    client->state = Client::State::Initialized;
    client->packet_handler.Register(PacketID::Login, PacketHandlers::HLogin::Main, data_ptr);
    client->packet_handler.Register(PacketID::Account, PacketHandlers::HAccount::Main, data_ptr);
    unsigned char answer = 1;

    sf::Packet reply;
    reply << (unsigned short)PacketID::Init;
    reply << answer;
    client->Send(reply);

    std::cout << "Client initialized." << std::endl;
    std::cout << "Client state: " << (int)client->state << std::endl;
}

}
