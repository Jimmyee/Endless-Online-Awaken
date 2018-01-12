#include "file_data.hpp"

#include "../map.hpp"

#include <fstream>
#include <string.h>

static std::string file_data = "";

namespace PacketHandlers::FileData
{
    void Main(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        unsigned char type = 0;

        packet >> type;

        if(type == 1)
        {
            FileTransfer(packet, data_ptr);
        }
    }

    void FileTransfer(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        unsigned char phase = 0;

        packet >> phase;

        if(phase == 1)
        {
            file_data = "";
        }
        if(phase == 2)
        {
            std::size_t psize = packet.getDataSize();

            char data_block[psize] = "";
            packet >> data_block;

            file_data += data_block;
        }
        if(phase == 3)
        {
            unsigned int id = 0;

            packet >> id;

            std::string filename = "data/maps/";
            filename += std::to_string(id) + ".map";

            std::ofstream file(filename, std::ios::trunc);
            if(!file.is_open())
            {
                throw std::runtime_error("Map: could not open file.");
            }
            else if(!file.good())
            {
                throw std::runtime_error("Map: data stream error.");
            }

            const char *cdata = file_data.c_str();
            file.write(cdata, (unsigned)strlen(cdata));
            file.close();

            file_data = "";

            Map map;

            std::vector<std::shared_ptr<Character>> characters = map.characters;
            map.Load(id);
            map.characters = characters;
        }
    }
}
