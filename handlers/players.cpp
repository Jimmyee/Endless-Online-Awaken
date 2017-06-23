// Endless Online Awaken v0.0.1

#include "handlers.hpp"
#include "../singleton.hpp"

void Players_Agree(PacketReader reader)
{
    S &s = S::GetInstance();

    reader.GetByte();
    shared_ptr<Character> character = shared_ptr<Character>(new Character());
    character->name = reader.GetBreakString();
    character->gameworld_id = reader.GetShort();
    reader.GetShort(); // map id
    character->x = reader.GetShort();
    character->y = reader.GetShort();
    character->direction = static_cast<Direction>(reader.GetChar());
    reader.GetChar(); // 6 (?)
    character->guild_tag = reader.GetFixedString(3);
    character->level = reader.GetChar();
    character->gender = static_cast<Gender>(reader.GetChar());
    character->hair_style = reader.GetChar();
    character->hair_color = reader.GetChar();
    character->race = static_cast<Skin>(reader.GetChar());
    character->max_hp = reader.GetShort();
    character->hp = reader.GetShort();
    character->max_tp = reader.GetShort();
    character->tp = reader.GetShort();

    // equipment
    for(int i = 0; i < 9; ++i)
    {
        reader.GetShort();
    }

    character->sitting = static_cast<SitState>(reader.GetChar());
    character->visibility = reader.GetChar();
    reader.GetByte(); // 255
    reader.GetByte(); // 0 = NPC, 1 = player

    s.map->characters.push_back(character);
}
