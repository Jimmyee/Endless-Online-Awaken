#include "handlers.hpp"
#include "../singleton.hpp"

// TODO:

void Welcome_Reply(PacketReader reader)
{
    S &s = S::GetInstance();

    s.eoclient->SetState(EOClient::State::Playing);
    s.gui->SetState(GUI::State::PlayGame);
}
