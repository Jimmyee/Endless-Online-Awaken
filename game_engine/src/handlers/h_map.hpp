#ifndef H_MAP_HPP_INCLUDED
#define H_MAP_HPP_INCLUDED

#include <SFML/Network.hpp>
#include <array>

namespace PacketHandlers::HMap
{
    void Main(sf::Packet &packet, std::array<intptr_t, 4> data_ptr);
    void Appear(sf::Packet &packet, std::array<intptr_t, 4> data_ptr);
}

#endif // H_MAP_HPP_INCLUDED
