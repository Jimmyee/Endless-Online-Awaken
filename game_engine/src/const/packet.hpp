// Endless Online Awaken

#ifndef CONST_HPP_INCLUDED
#define CONST_HPP_INCLUDED

#include <SFML/Network.hpp>

enum class PacketID : unsigned char
{
    Init,
    Login,
    Account,
    Character,
    NPC,
    Map,
    FileData,
    Ping
};

#endif // CONST_HPP_INCLUDED
