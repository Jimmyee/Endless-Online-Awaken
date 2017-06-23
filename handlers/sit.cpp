// Endless Online Awaken v0.0.1

#include "handlers.hpp"
#include "../singleton.hpp"

void Sit_Player(PacketReader reader)
{
    S &s = S::GetInstance();

    short gameworld_id = reader.GetShort();

    shared_ptr<Character> character = s.map->GetCharacter(gameworld_id);
    if(!character.get()) return;

    character->x = reader.GetChar();
    character->y = reader.GetChar();
    character->direction = static_cast<Direction>(reader.GetChar());
    reader.GetChar(); // 0 (?)
    character->sitting = SitState::Floor;
}

void Sit_Remove(PacketReader reader)
{
    S &s = S::GetInstance();

    short gameworld_id = reader.GetShort();

    shared_ptr<Character> character = s.map->GetCharacter(gameworld_id);
    if(!character.get())return;

    character->x = reader.GetChar();
    character->y = reader.GetChar();
    character->sitting = SitState::Stand;

    s.eoclient->Talk(std::string() + "Hey you, " + character->name + "! Did you just stand up? Please sit back on your place!");
}
