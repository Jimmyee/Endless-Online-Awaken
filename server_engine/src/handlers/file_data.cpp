#include "file_data.hpp"

#include "../file_handler.hpp"
#include "../const/packet.hpp"
#include "../client.hpp"

#include <string.h>

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
        Client *client = (Client *)data_ptr[0];
        unsigned int id = 0;

        packet >> id;

        std::string filename = "data/maps/" + std::to_string(id) + ".map";

        FileHandler fh(filename);

        if(fh.Exists())
        {
            std::string data = fh.GetData();

            sf::Packet reply;

            reply << (unsigned short)PacketID::FileData;
            reply << (unsigned char)1;
            reply << (unsigned char)1;

            client->Send(reply);

            client->transfer_data = fh.GetData();
            client->transfer_id = id;
            client->transfer_clock.restart();
        }
    }
}
