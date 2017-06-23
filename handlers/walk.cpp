// Endless Online Awaken v0.0.1

#include "handlers.hpp"
#include "../singleton.hpp"

void Walk_Player(PacketReader reader)
{
    S &s = S::GetInstance();

    short gameworld_id = reader.GetShort();

    shared_ptr<Character> character = s.map->GetCharacter(gameworld_id);
    if(character.get())
    {
        character->direction = static_cast<Direction>(reader.GetChar());
        character->x = reader.GetChar();
        character->y = reader.GetChar();
    }
}
