#include "handlers.hpp"
#include "../singleton.hpp"

void Refresh_Reply(PacketReader reader)
{
    S &s = S::GetInstance();

    unsigned char player_amount = reader.GetChar();
    reader.GetByte(); // 255


    s.map.characters.clear();
    s.map.npcs.clear();
    for(int i = 0; i < player_amount; ++i)
    {
        Character *character;
        std::string name = reader.GetBreakString();
        if(name == s.character.name)
        {
            character = &s.character;
        }
        else
        {
            character = new Character();
            character->name = name;
        }

        character->gameworld_id = reader.GetShort();
        reader.GetShort(); // map id
        character->x = reader.GetShort();
        character->y = reader.GetShort();
        character->direction = static_cast<Direction>(reader.GetChar());
        reader.GetChar(); // ?
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
        for(int pi = 0; pi < 9; ++pi)
        {
            reader.GetShort(); // item id
        }

        character->sitting = static_cast<SitState>(reader.GetChar());
        character->visibility = reader.GetChar();
        reader.GetByte(); // 255

        s.map.characters.push_back(*character);
    }

    while(reader.PeekByte() != 255)
    {
        NPC npc;
        npc.gameworld_index = reader.GetChar();
        npc.id = reader.GetShort();
        npc.x = reader.GetChar();
        npc.y = reader.GetChar();
        npc.direction = static_cast<Direction>(reader.GetChar());
        npc.data = shared_ptr<ENF_Data>(new ENF_Data(s.enf->Get(npc.id)));

        s.map.npcs.push_back(npc);
    }
    reader.GetByte();
    // TODO: read items on the map
}
