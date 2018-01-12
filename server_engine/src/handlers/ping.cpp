#include "ping.hpp"

#include "../client.hpp"

#include <iostream>

namespace PacketHandlers::HPing
{
    void Main(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        Client *client = (Client *)data_ptr[0];

        client->latency = client->ping_clock.getElapsedTime().asMilliseconds();

        client->ping_clock.restart();
    }
}
