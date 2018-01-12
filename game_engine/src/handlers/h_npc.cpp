#include "h_npc.hpp"

#include "../client.hpp"
#include "../map.hpp"

#include <iostream>

namespace PacketHandlers::HNPC
{
    void Main(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        unsigned char sub_id = 0;
        packet >> sub_id;

        if(sub_id == 1) // npc list
        {
            List(packet, data_ptr);
        }
        if(sub_id == 2) // get in range
        {

        }
        if(sub_id == 3) // talk
        {
            Talk(packet, data_ptr);
        }
        if(sub_id == 4) // face
        {
            Face(packet, data_ptr);
        }
        if(sub_id == 5) // walk
        {
            Walk(packet, data_ptr);
        }
    }

    void List(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        std::size_t npcs = 0;

        packet >> npcs;

        Map().npcs.clear();

        for(std::size_t i = 0; i < npcs; ++i)
        {
            NPC *npc = new NPC();

            packet >> npc->id;
            packet >> npc->index;
            packet >> npc->map_id;
            packet >> npc->x;
            packet >> npc->y;
            unsigned char dir = 0;
            packet >> dir;
            npc->direction = (Direction)dir;
            packet >> npc->speed;

            Map().npcs.push_back(std::shared_ptr<NPC>(npc));
        }
    }

    void Talk(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        unsigned int index = 0;
        std::string message = "";

        packet >> index;
        packet >> message;

        NPC *npc = Map().GetNPC(index);

        if(npc == 0) return;

        npc->Talk(message);
    }

    void Face(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        unsigned int index = 0;
        unsigned char direction = 0;

        packet >> index;
        packet >> direction;

        NPC *npc = Map().GetNPC(index);

        if(npc == 0) return;

        npc->Face((Direction)direction);
    }

    void Walk(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        unsigned char index = 0;
        unsigned char direction = 0;
        unsigned short x = 0;
        unsigned short y = 0;

        packet >> index;
        packet >> direction;
        packet >> x;
        packet >> y;

        NPC *npc = Map().GetNPC(index);

        if(npc == 0)
        {
            Client().GetInRange(index);

            return;
        }

        npc->Walk((Direction)direction);

        npc->direction = (Direction)direction;
        npc->x = x;
        npc->y = y;
    }
}
