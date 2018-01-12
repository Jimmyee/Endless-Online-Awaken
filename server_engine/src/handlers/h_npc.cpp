#include "h_npc.hpp"

#include "../client.hpp"
#include "../map.hpp"
#include "../map_handler.hpp"

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
        if(sub_id == 2) // get npc in range by id
        {
            GetInRange(packet, data_ptr);
        }
    }

    void List(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        Client *client = (Client*)data_ptr[0];
        Map *map = MapHandler().GetMap(client->map_id);

        sf::Packet reply;

        reply << (unsigned char)PacketID::NPC;
        reply << (unsigned char)1; // npc list
        reply << map->npcs.size();
        for(auto &npc : map->npcs)
        {
            reply << npc->id;
            reply << npc->index;
            reply << npc->map_id;
            reply << npc->x;
            reply << npc->y;
            reply << (unsigned char)npc->direction;
            reply << npc->speed;
        }

        client->Send(reply);
    }

    void GetInRange(sf::Packet packet, std::array<intptr_t, 4> data_ptr)
    {
        Client *client = (Client*)data_ptr[0];
        unsigned int index = 0;

        packet >> index;

        Map *map = MapHandler().GetMap(client->map_id);
        NPC *npc = map->GetNPC(index);

        if(npc == 0) return;

        sf::Packet reply;

        reply << (unsigned char)PacketID::Map;
        reply << (unsigned char)1; // appear
        reply << (unsigned char)2; // npc
        reply << npc->id;
        reply << npc->index;
        reply << npc->map_id;
        reply << npc->x;
        reply << npc->y;
        reply << (unsigned char)npc->direction;
        reply << npc->speed;

        client->Send(reply);
    }
}
