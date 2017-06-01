#ifndef HANDLERS_HPP_INCLUDED
#define HANDLERS_HPP_INCLUDED

#include "../packet.hpp"

class EOClient;

void INIT_INIT(EOClient *client, PacketReader reader);
void Connection_Player(EOClient *client, PacketReader reader);

#endif // HANDLERS_HPP_INCLUDED
