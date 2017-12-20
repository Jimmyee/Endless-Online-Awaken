#ifndef ACCOUNT_HPP_INCLUDED
#define ACCOUNT_HPP_INCLUDED

#include <SFML/Network.hpp>
#include <array>

namespace PacketHandlers::HAccount
{
    void Main(sf::Packet &packet, std::array<intptr_t, 4> data_ptr);
    void Create(sf::Packet &packet, std::array<intptr_t, 4> data_ptr);
    void Delete(sf::Packet &packet, std::array<intptr_t, 4> data_ptr);
};

#endif // ACCOUNT_HPP_INCLUDED
