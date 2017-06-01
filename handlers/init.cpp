#include "handlers.hpp"
#include "../eoclient.hpp"
#include "../packet.hpp"
#include "../const/init.hpp"

#include <cstdio>

void INIT_INIT(EOClient *client, PacketReader reader)
{
    InitReply result(static_cast<InitReply>(reader.GetByte()));
    if(result == INIT_OK)
    {
        client->Initialize(reader);
    }
    else
    {
        puts("EOClient: init failed");
        client->Disconnect();
    }
}
