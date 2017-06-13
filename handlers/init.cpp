#include "handlers.hpp"
#include "../packet.hpp"
#include "../singleton.hpp"
#include "../const/init.hpp"
#include "../gui.hpp"

#include <cstdio>

void INIT_INIT(PacketReader reader)
{
    S &s = S::GetInstance();

    InitReply result(static_cast<InitReply>(reader.GetByte()));

    if(result == InitReply::OK)
    {
        s.eoclient->Initialize(reader);

        s.eoclient->RegisterHandler(PacketFamily::Connection, PacketAction::Player, Connection_Player);
        s.eoclient->RegisterHandler(PacketFamily::Login, PacketAction::Reply, Login_Reply);
        s.eoclient->RegisterHandler(PacketFamily::Account, PacketAction::Reply, Account_Reply);
        s.eoclient->RegisterHandler(PacketFamily::Welcome, PacketAction::Reply, Welcome_Reply);

        PacketBuilder packet(PacketFamily::Connection, PacketAction::Accept);
        s.eoclient->Send(packet);
    }
    else
    {
        puts("EOClient: init failed");
        s.eoclient->Disconnect();
    }
}
