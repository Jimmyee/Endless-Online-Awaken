#ifndef LOGIN_HPP_INCLUDED
#define LOGIN_HPP_INCLUDED

#include <SFML/Network.hpp>

namespace PacketHandlers::HLogin
{
    void Main(sf::Packet &packet, std::array<intptr_t, 4> data_ptr);
};

#endif // LOGIN_HPP_INCLUDED
