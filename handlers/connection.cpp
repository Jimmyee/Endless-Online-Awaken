// Endless Online Awaken v0.0.1

#include "handlers.hpp"
#include "../singleton.hpp"
#include "../packet.hpp"

void Connection_Player(PacketReader reader)
{
    S &s = S::GetInstance();

    unsigned short s1 = reader.GetShort();
    unsigned char s2 = reader.GetChar();

    printf("Connection_Player: s1: %i, s2: %i\n", s1, s2);

    s.eoclient->UpdateSequenceByte(s1, s2);

    PacketBuilder reply(PacketFamily::Connection, PacketAction::Ping);
    s.eoclient->Send(reply);
}
