#include "h_map.hpp"

#include "../client.hpp"
#include "../map.hpp"

#include <iostream>

namespace PacketHandlers::HMap
{
    void Main(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        unsigned char sub_id = 0;

        packet >> sub_id;

        if(sub_id == 1)
        {
            Appear(packet, data_ptr);
        }
    }

    void Appear(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        unsigned char sub_id = 0;
        packet >> sub_id;

        std::shared_ptr<Character> character = std::shared_ptr<Character>(new Character());
        unsigned char char_buf = 0;
        packet >> character->name;
        packet >> character->map_id;
        packet >> character->x;
        packet >> character->y;
        packet >> char_buf; character->direction = (Direction)char_buf;
        packet >> char_buf; character->gender = (Gender)char_buf;

        Map().characters.push_back(character);

        std::cout << "Character appeared: " << character->name << " " << character->x << "x" << character->y << std::endl;
    }
}
