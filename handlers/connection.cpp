#include "handlers.hpp"
#include "../eoclient.hpp"
#include "../packet.hpp"

void Connection_Player(EOClient *client, PacketReader reader)
{
    unsigned short s1 = reader.GetShort();
    unsigned char s2 = reader.GetChar();

    client->UpdateSequenceByte(s1, s2);

    PacketBuilder reply(PacketFamily::PACKET_CONNECTION, PacketAction::PACKET_PING);
    client->Send(reply);
}
