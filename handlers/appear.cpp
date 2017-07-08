// Endless Online Awaken v0.0.1

#include "handlers.hpp"
#include "../singleton.hpp"
#include "../eodata.hpp"

// NPC appears in range
void Appear_Reply(PacketReader reader)
{
    S &s = S::GetInstance();

    reader.GetChar(); // 0
    reader.GetChar(); // 255
    NPC npc;
    npc.gameworld_index = reader.GetChar();
    npc.id = reader.GetShort();
    npc.x = reader.GetChar();
    npc.y = reader.GetChar();
    npc.direction = static_cast<Direction>(reader.GetChar());
    npc.data = shared_ptr<ENF_Data>(new ENF_Data(s.enf->Get(npc.id)));
    s.map.npcs.push_back(npc);
}
