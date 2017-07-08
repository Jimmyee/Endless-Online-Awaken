// Endless Online Awaken v0.0.1

#include "handlers.hpp"
#include "../singleton.hpp"

void Sit_Player(PacketReader reader)
{
    S &s = S::GetInstance();

    short gameworld_id = reader.GetShort();

    int i = s.map.GetCharacterIndex(gameworld_id);
    if(i == -1) return;

    s.map.characters[i].x = reader.GetChar();
    s.map.characters[i].y = reader.GetChar();
    s.map.characters[i].direction = static_cast<Direction>(reader.GetChar());
    reader.GetChar(); // 0 (?)
    s.map.characters[i].sitting = SitState::Floor;

    if(s.map.characters[i].gameworld_id == s.character.gameworld_id)
    {
        s.character.sitting = s.map.characters[i].sitting;
    }
}

void Sit_Close(PacketReader reader)
{
    S &s = S::GetInstance();

    short gameworld_id = reader.GetShort();

    int i = s.map.GetCharacterIndex(gameworld_id);
    if(i == -1) return;

    s.map.characters[i].x = reader.GetChar();
    s.map.characters[i].y = reader.GetChar();
    s.map.characters[i].sitting = SitState::Stand;

    if(s.map.characters[i].gameworld_id == s.character.gameworld_id)
    {
        s.character.sitting = s.map.characters[i].sitting;
    }
}
