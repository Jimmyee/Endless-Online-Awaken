#include "ping.hpp"

#include "../client.hpp"

namespace PacketHandlers::HPing
{
    void Main(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        sf::Packet reply;

        reply << (unsigned char)PacketID::Ping;

        Client().Send(reply);
    }
}
