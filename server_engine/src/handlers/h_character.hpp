#ifndef H_CHARACTER_HPP_INCLUDED
#define H_CHARACTER_HPP_INCLUDED

#include <SFML/Network.hpp>
#include <array>

namespace PacketHandlers::HCharacter
{
    void Main(sf::Packet &packet, std::array<intptr_t, 4> data_ptr);
    void Create(sf::Packet &packet, std::array<intptr_t, 4> data_ptr);
    void Delete(sf::Packet &packet, std::array<intptr_t, 4> data_ptr);
    void List(sf::Packet &packet, std::array<intptr_t, 4> data_ptr);
    void Select(sf::Packet &packet, std::array<intptr_t, 4> data_ptr);

    void Talk(sf::Packet &packet, std::array<intptr_t, 4> data_ptr);
    void Face(sf::Packet &packet, std::array<intptr_t, 4> data_ptr);
    void Walk(sf::Packet &packet, std::array<intptr_t, 4> data_ptr);

    void GetInRange(sf::Packet packet, std::array<intptr_t, 4> data_ptr);
};

#endif // H_CHARACTER_HPP_INCLUDED
