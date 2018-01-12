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
        unsigned char buf = 0;
        unsigned char entity = 0;

        packet >> entity;

        if(entity == 1) // character
        {
            Character *character = 0;
            std::string name = "";

            packet >> name;

            character = Map().GetCharacter(name);

            if(character == 0)
            {
                character = new Character();
                character->name = name;
                Map().characters.push_back(std::shared_ptr<Character>(character));
            }

            packet >> character->map_id;
            packet >> character->x;
            packet >> character->y;
            packet >> buf; character->direction = (Direction)buf;
            packet >> buf; character->gender = (Gender)buf;
            packet >> character->speed;

            std::cout << "Hello " << character->name << "." << std::endl;
        }
        if(entity == 2) // npc
        {
            unsigned int id = 0;
            unsigned int index = 0;

            packet >> id;
            packet >> index;

            NPC *npc = Map().GetNPC(index);

            if(npc == 0)
            {
                npc = new NPC();
                npc->id = id;
                npc->index = index;
                Map().npcs.push_back(std::shared_ptr<NPC>(npc));
            }

            packet >> npc->map_id;
            packet >> npc->x;
            packet >> npc->y;
            packet >> buf; npc->direction = (Direction)buf;
            packet >> npc->speed;
        }
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
