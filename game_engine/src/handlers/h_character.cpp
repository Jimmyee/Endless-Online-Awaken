// Endless Online Awaken

#include "h_character.hpp"

#include "h_map.hpp"
#include "h_npc.hpp"
#include "file_data.hpp"
#include "../client.hpp"
#include "../gui.hpp"
#include "../map.hpp"
#include "../game_state.hpp"

#include <iostream>

namespace PacketHandlers::HCharacter
{
    void Main(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        Client client;

        unsigned char sub_id = 0;

        packet >> sub_id;

        if(sub_id == 1) // create character
        {
            Create(packet, data_ptr);
        }
        if(sub_id == 2)
        {
            Delete(packet, data_ptr);
        }
        if(sub_id == 3) // character list
        {
            List(packet, data_ptr);
        }
        if(sub_id == 4) // select character
        {
            Select(packet, data_ptr);
        }
        if(sub_id == 5)
        {
            Talk(packet, data_ptr);
        }
        if(sub_id == 6)
        {
            Face(packet, data_ptr);
        }
        if(sub_id == 7)
        {
            Walk(packet, data_ptr);
        }
    }

    void Create(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        Client client;
        unsigned char answer = 0;
        std::string message;

        packet >> answer;
        packet >> message;

        if(answer == 1)
        {
            std::size_t characters = 0;

            packet >> characters;

            client.characters.clear();
            for(std::size_t i = 0; i < characters; ++i)
            {
                std::shared_ptr<Character> character = std::shared_ptr<Character>(new Character());

                packet >> character->name;
                unsigned char gender = 0;
                packet >> gender;
                character->gender = (Gender)gender;

                client.characters.push_back(character);
            }


            GUI().OpenPopup("Character created");
        }
        else
        {
            GUI().OpenPopup("Server answer", message);
        }
    }

    void Delete(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        Client client;
        unsigned char answer = 0;
        std::string message = "";

        packet >> answer;
        packet >> message;

        if(answer == 1)
        {
            std::string name = "";
            packet >> name;

            client.RemoveCharacter(name);
        }

        GUI().OpenPopup("Server answer", message);
    }

    void List(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        Client client;

        std::size_t slots = 0;
        packet >> slots;
        for(std::size_t i = 0; i < slots; ++i)
        {
            std::string name = "";
            unsigned char gender = 0;

            packet >> name;
            packet >> gender;

            std::shared_ptr<Character> character = std::shared_ptr<Character>(new Character());
            character->name = name;
            character->gender = (Gender)gender;

            client.characters.push_back(character);
        }
    }

    void Select(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        Client client;
        unsigned char answer = 0;
        std::string message = "";

        packet >> answer;
        packet >> message;

        if(answer == 1)
        {
            Map map;

            client.state = Client::State::Playing;
            GUI().SetState(GUI::State::Playing);
            GameState().Set(GameState::State::Playing);

            client.packet_handler.Register(PacketID::Map, PacketHandlers::HMap::Main, data_ptr);
            client.packet_handler.Register(PacketID::NPC, PacketHandlers::HNPC::Main, data_ptr);
            client.packet_handler.Register(PacketID::FileData, PacketHandlers::FileData::Main, data_ptr);

            Character character;
            packet >> character.name;
            packet >> character.map_id;

            map.Load(character.map_id);

            std::size_t chars_in_range = 0;

            packet >> chars_in_range;

            for(std::size_t i = 0; i < chars_in_range; ++i)
            {
                std::shared_ptr<Character> character = std::shared_ptr<Character>(new Character());

                unsigned char buf = 0;
                packet >> character->name;
                packet >> character->map_id;
                packet >> character->x;
                packet >> character->y;
                packet >> buf; character->direction = (Direction)buf;
                packet >> buf; character->gender = (Gender)buf;
                packet >> character->speed;

                map.characters.push_back(character);
            }

            client.character = map.GetCharacter(character.name);

            std::size_t npcs_in_range = 0;

            packet >> npcs_in_range;

            for(std::size_t i = 0; i < npcs_in_range; ++i)
            {
                std::shared_ptr<NPC> npc = std::shared_ptr<NPC>(new NPC());

                packet >> npc->id;
                packet >> npc->index;
                packet >> npc->map_id;
                packet >> npc->x;
                packet >> npc->y;
                unsigned char dir = 0;
                packet >> dir; npc->direction = (Direction)dir;
                packet >> npc->speed;

                map.npcs.push_back(npc);
            }
        }
        if(answer == 2)
        {
            unsigned int revision = 0;
            Map map;

            packet >> revision;

            while(!packet.endOfPacket())
            {
                std::string message = "";

                packet >> message;

                Chat().AddMessage("Server", message);
            }

            unsigned char answer = 0;

            if(revision == map.revision)
            {
                answer = 1;
            }
            else
            {
                answer = 2;
            }

            if(answer == 2)
            {
                sf::Packet reply;

                reply << (unsigned char)PacketID::FileData;
                reply << (unsigned char)1; // map file
                reply << map.id;

                client.Send(reply);
            }
        }

        std::cout << message << std::endl;
    }

    void Talk(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        unsigned char channel = 0;
        std::string char_name = "";
        std::string message = "";

        packet >> channel;
        packet >> char_name;
        packet >> message;

        Chat().AddMessage(char_name, message);
    }

    void Face(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        std::string name = "";
        unsigned short source_x = 0;
        unsigned short source_y = 0;
        unsigned char buf = 0;

        packet >> name;
        packet >> buf;
        Direction direction = (Direction)buf;
        packet >> source_x;
        packet >> source_y;

        Character *character = Map().GetCharacter(name);

        if(character == 0)
        {
            Client().GetInRange(name);
            return;
        }

        if(character->anim_state != Character::AnimState::Walk && character->anim_state != Character::AnimState::Attack)
        {
            character->direction = direction;
            character->x = source_x;
            character->y = source_y;
        }

        character->Face(direction);
    }

    void Walk(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        std::string name = "";
        unsigned short source_x = 0;
        unsigned short source_y = 0;
        unsigned char buf = 0;

        packet >> name;
        packet >> buf;
        Direction direction = (Direction)buf;
        packet >> source_x;
        packet >> source_y;

        Character *character = Map().GetCharacter(name);

        if(character == 0)
        {
            Client().GetInRange(name);
            return;
        }

        character->direction = direction;
        character->x = source_x;
        character->y = source_y;

        if(character->name == Client().character->name)
        {
            if(character->direction == Direction::Up) character->y--;
            if(character->direction == Direction::Right) character->x++;
            if(character->direction == Direction::Down) character->y++;
            if(character->direction == Direction::Left) character->x--;
        }

        character->Walk(direction);
    }
}
