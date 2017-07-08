#include "handlers.hpp"
#include "../singleton.hpp"

void NPC_Spec(PacketReader reader)
{
    S &s = S::GetInstance();

    reader.GetShort(); // killer gameworld_id
    reader.GetChar(); // killer direction
    short index = reader.GetShort();

    // TODO: read the rest or not
    if(reader.Remaining() > 0)
    {

    }

    s.map.RemoveNPC(index);
}
