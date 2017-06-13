#ifndef HANDLERS_HPP_INCLUDED
#define HANDLERS_HPP_INCLUDED

#include "../packet.hpp"

void INIT_INIT(PacketReader reader);
void Connection_Player(PacketReader reader);
void Login_Reply(PacketReader reader);
void Account_Reply(PacketReader reader);
void Welcome_Reply(PacketReader reader);

#endif // HANDLERS_HPP_INCLUDED
