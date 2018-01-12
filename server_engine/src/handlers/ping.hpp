#ifndef PING_HPP_INCLUDED
#define PING_HPP_INCLUDED

#include <SFML/Network.hpp>
#include <array>

namespace PacketHandlers::HPing
{
    void Main(sf::Packet &packet, std::array<intptr_t, 4> data_ptr);
}

#endif // PING_HPP_INCLUDED
