#ifndef FILE_DATA_HPP_INCLUDED
#define FILE_DATA_HPP_INCLUDED

#include <SFML/Network.hpp>

namespace PacketHandlers::FileData
{
    void Main(sf::Packet &packet, std::array<intptr_t, 4> data_ptr);
    void FileTransfer(sf::Packet &packet, std::array<intptr_t, 4> data_ptr);
}

#endif // FILE_DATA_HPP_INCLUDED
