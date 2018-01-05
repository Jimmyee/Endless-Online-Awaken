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
        if(sub_id == 2)
        {
            Leave(packet, data_ptr);
        }
    }

    void Appear(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        Character *character = 0;
        std::string name = "";

        std::cout << "APPEAR\n";

        unsigned char char_buf = 0;
        packet >> name;

        character = Map().GetCharacter(name);

        if(character == 0)
        {
            character = new Character();
            character->name = name;
            Map().characters.push_back(std::shared_ptr<Character>(character));

            std::cout << "total characters: " << Map().characters.size() << std::endl;
        }

        packet >> character->map_id;
        packet >> character->x;
        packet >> character->y;
        packet >> char_buf; character->direction = (Direction)char_buf;
        packet >> char_buf; character->gender = (Gender)char_buf;

        //Map().characters.push_back(character);
    }

    void Leave(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        std::string name = "";

        packet >> name;

        Map map;
        Character *character = map.GetCharacter(name);

        if(character == 0) return;

        map.DeleteCharacter(name);
    }
}
