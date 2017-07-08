// Endless Online Awaken v0.0.1

#ifndef HANDLERS_HPP_INCLUDED
#define HANDLERS_HPP_INCLUDED

#include "../packet.hpp"

void INIT_INIT(PacketReader reader);
void Connection_Player(PacketReader reader);
void Login_Reply(PacketReader reader);
void Account_Reply(PacketReader reader);
void Welcome_Reply(PacketReader reader);

void Refresh_Reply(PacketReader reader);

void Avatar_Remove(PacketReader reader); // Character disappears
void Players_Agree(PacketReader reader); // Characters appears in range
void Walk_Player(PacketReader reader);

void Talk_Player(PacketReader reader); // public message

void Sit_Player(PacketReader reader);
void Sit_Close(PacketReader reader);

void Trade_Request(PacketReader reader);
void Trade_Open(PacketReader reader);
void Trade_Close(PacketReader reader);
void Trade_Reply(PacketReader reader);
void Trade_Spec(PacketReader reader);
void Trade_Agree(PacketReader reader);
void Trade_Use(PacketReader reader);

void Appear_Reply(PacketReader reader); // NPC appears in range
void NPC_Spec(PacketReader reader);

#endif // HANDLERS_HPP_INCLUDED
