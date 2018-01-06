// Endless Online Awaken

#ifndef CONST_HPP_INCLUDED
#define CONST_HPP_INCLUDED

#include <SFML/Network.hpp>

enum class PacketID : unsigned short
{
    Init,
    Login,
    Account,
    Character,
    Map,
    FileData
};

#endif // CONST_HPP_INCLUDED
