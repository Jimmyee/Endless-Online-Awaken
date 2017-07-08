// Endless Online Awaken v0.0.1

#include "handlers.hpp"
#include "../singleton.hpp"
#include "../eodata.hpp"
#include "../character.hpp"

void Welcome_Reply(PacketReader reader)
{
    S &s = S::GetInstance();

    int sub_id = reader.GetShort();

    if(sub_id != 1 && sub_id != 2)
    {
        std::string title = "Something is not good";
        std::string message = "It is unable to use this character at this moment.";

        s.gui.popup_modal = shared_ptr<GUI::PopupModal>(new GUI::PopupModal("msg_select_char", title, message, 0));
        return;
    }

    if(sub_id == 1)
    {
        s.character.gameworld_id = reader.GetShort();
        unsigned int character_id = reader.GetInt();
        if(character_id != s.character.id)
        {
            s.gui.popup_modal = shared_ptr<GUI::PopupModal>(new GUI::PopupModal("msg_exception", "Error", "Character could not be loaded.", 0));
            return;
        }

        short map_id = reader.GetShort();
        s.emf = shared_ptr<EMF>(new EMF(map_id));
        if(s.emf->exists)
        {
            s.map.Reset();
            printf("Map [%i] loaded.\n", map_id);
        }
        else
        {
            printf("Could not load map [%i].\n", map_id);
        }

        std::array<unsigned char, 4> map_rid;
        PacketReader reader_copy(reader);
        std::string remaining_data = reader_copy.GetEndString();
        if(remaining_data[0] == 0xFF)
        {
            reader.GetByte(); // 0xFF
            reader.GetByte(); // PK information
        }
        else
        {
            map_rid[0] = reader.GetByte();
            map_rid[1] = reader.GetByte();
        }

        map_rid[2] = reader.GetByte();
        map_rid[3] = reader.GetByte();
        reader.GetThree(); // map file size

        std::array<unsigned char, 4> pub_rid;
        std::array<unsigned char, 2> pub_len;

        for(int i = 0; i < 4; ++i)
        {
            pub_rid[0] = reader.GetByte();
            pub_rid[1] = reader.GetByte();
            pub_rid[2] = reader.GetByte();
            pub_rid[3] = reader.GetByte();
            pub_len[0] = reader.GetByte();
            pub_len[1] = reader.GetByte();
        }

        s.character.name = reader.GetBreakString();
        s.character.title = reader.GetBreakString();
        s.character.guild = reader.GetBreakString();
        s.character.guild_rank = reader.GetBreakString();
        s.character.clas = reader.GetChar();
        s.character.guild_tag = reader.GetFixedString(3);
        s.character.admin_level = static_cast<AdminLevel>(reader.GetChar());

        s.character.level = reader.GetChar();
        s.character.exp = reader.GetInt();
        s.character.usage = reader.GetInt();
        s.character.hp = reader.GetShort();
        s.character.max_hp = reader.GetShort();
        s.character.tp = reader.GetShort();
        s.character.max_tp = reader.GetShort();
        s.character.max_sp = reader.GetShort();
        s.character.stat_points = reader.GetShort();
        s.character.karma = reader.GetShort();
        s.character.min_dam = reader.GetShort();
        s.character.max_dam = reader.GetShort();
        s.character.accuracy = reader.GetShort();
        s.character.evade = reader.GetShort();
        s.character.armor = reader.GetShort();

        s.character.str = reader.GetShort();
        s.character.wis = reader.GetShort();
        s.character.intl = reader.GetShort();
        s.character.agi = reader.GetShort();
        s.character.con = reader.GetShort();
        s.character.cha = reader.GetShort();

        // TODO: read rest of the data

        PacketBuilder packet(PacketFamily::Welcome, PacketAction::Message);
        packet.AddThree(1);
        packet.AddInt(s.character.id);
        s.eoclient.Send(packet);
    }
    else if(sub_id == 2)
    {
        s.eoclient.SetState(EOClient::State::Playing);
        s.gui.SetState(GUI::State::PlayGame);

        reader.GetByte(); // 255

        for (int i = 0; i < 9; ++i)
        {
            reader.GetBreakString();
        }

        s.character.weight = reader.GetChar();
        s.character.max_weight = reader.GetChar();

        s.inventory.Clear();
        while(reader.PeekByte() != 255)
        {
            short item_id = reader.GetShort();
            int item_amount = reader.GetInt();
            s.inventory.AddItem(item_id, item_amount);
        }
        reader.GetByte(); // 255

        while(reader.PeekByte() != 255)
        {
            reader.GetShort(); // spell id
            reader.GetShort(); // spell level
        }
        reader.GetByte(); // 255

        unsigned char player_amount = reader.GetChar();
        reader.GetByte(); // 255
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
        // TODO: read the rest

        s.eoclient.UnregisterHandler(PacketFamily::Login, PacketAction::Reply);
        s.eoclient.UnregisterHandler(PacketFamily::Account, PacketAction::Reply);
        s.eoclient.UnregisterHandler(PacketFamily::Welcome, PacketAction::Reply);

        s.eoclient.RegisterHandler(PacketFamily::Refresh, PacketAction::Reply, Refresh_Reply);

        s.eoclient.RegisterHandler(PacketFamily::Players, PacketAction::Agree, Players_Agree);
        s.eoclient.RegisterHandler(PacketFamily::Avatar, PacketAction::Remove, Avatar_Remove);
        s.eoclient.RegisterHandler(PacketFamily::Walk, PacketAction::Player, Walk_Player);
        s.eoclient.RegisterHandler(PacketFamily::Talk, PacketAction::Player, Talk_Player);
        s.eoclient.RegisterHandler(PacketFamily::Sit, PacketAction::Player, Sit_Player);
        s.eoclient.RegisterHandler(PacketFamily::Sit, PacketAction::Close, Sit_Close);

        s.eoclient.RegisterHandler(PacketFamily::Trade, PacketAction::Request, Trade_Request);
        s.eoclient.RegisterHandler(PacketFamily::Trade, PacketAction::Open, Trade_Open);
        s.eoclient.RegisterHandler(PacketFamily::Trade, PacketAction::Close, Trade_Close);
        s.eoclient.RegisterHandler(PacketFamily::Trade, PacketAction::Reply, Trade_Reply);
        s.eoclient.RegisterHandler(PacketFamily::Trade, PacketAction::Spec, Trade_Spec);
        s.eoclient.RegisterHandler(PacketFamily::Trade, PacketAction::Agree, Trade_Agree);
        s.eoclient.RegisterHandler(PacketFamily::Trade, PacketAction::Use, Trade_Use);


        s.eoclient.RegisterHandler(PacketFamily::Appear, PacketAction::Reply, Appear_Reply);
        s.eoclient.RegisterHandler(PacketFamily::NPC, PacketAction::Spec, NPC_Spec);
    }

    //s.eoclient.Talk("[EOAwaken v0.0.1] For god's sake, I don't even see anything on the screen!");
}
