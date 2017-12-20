#include "h_map.hpp"

namespace PacketHandlers::HMap
{
    void Main(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        unsigned char sub_id = 0;

        packet >> sub_id;

        if(sub_id == 1) // an entity appears
        {

        }
    }
}
