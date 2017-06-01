#include "handlers.hpp"
#include "../packet.hpp"
#include "../singleton.hpp"
#include "../const/init.hpp"

#include <cstdio>

void INIT_INIT(PacketReader reader)
{
    shared_ptr<EOClient> eoclient = S::GetInstance().eoclient;

    InitReply result(static_cast<InitReply>(reader.GetByte()));

    if(result == INIT_OK)
    {
        eoclient->Initialize(reader);
    }
    else
    {
        puts("EOClient: init failed");
        eoclient->Disconnect();
    }
}
