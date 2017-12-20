#ifndef INIT_HPP_INCLUDED
#define INIT_HPP_INCLUDED

#include <SFML/Network.hpp>

namespace PacketHandlers::HInit
{
    void Main(sf::Packet &packet, std::array<intptr_t, 4> data_ptr);
};

#endif // INIT_HPP_INCLUDED
