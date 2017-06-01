#include "handlers.hpp"
#include "../singleton.hpp"
#include "../packet.hpp"

void Connection_Player(PacketReader reader)
{
    shared_ptr<EOClient> eoclient = S::GetInstance().eoclient;

    unsigned short s1 = reader.GetShort();
    unsigned char s2 = reader.GetChar();

    eoclient->UpdateSequenceByte(s1, s2);

    PacketBuilder reply(PacketFamily::PACKET_CONNECTION, PacketAction::PACKET_PING);
    eoclient->Send(reply);
}
