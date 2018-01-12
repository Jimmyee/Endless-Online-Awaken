#ifndef H_NPC_HPP_INCLUDED
#define H_NPC_HPP_INCLUDED

#include <SFML/Network.hpp>
#include <array>

namespace PacketHandlers::HNPC
{
    void Main(sf::Packet &packet, std::array<intptr_t, 4> data_ptr);
    void List(sf::Packet &packet, std::array<intptr_t, 4> data_ptr);

    void GetInRange(sf::Packet packet, std::array<intptr_t, 4> data_ptr);
};

#endif // H_NPC_HPP_INCLUDED
