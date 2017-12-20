// Endless Online Awaken

#include "h_character.hpp"

#include "h_map.hpp"
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
            Face(packet, data_ptr);
        }
        if(sub_id == 6)
        {
            Talk(packet, data_ptr);
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

            client.packet_handler.Register(PacketID::Map, PacketHandlers::HMap::Appear, data_ptr);

            Character character;
            unsigned char char_buf = 0;
            packet >> character.name;
            packet >> character.map_id;
            packet >> character.x;
            packet >> character.y;
            packet >> char_buf; character.direction = (Direction)char_buf;
            packet >> char_buf; character.gender = (Gender)char_buf;

            map.Load(character.map_id);
            //map.characters.push_back(character);

            std::size_t chars_in_range = 0;

            packet >> chars_in_range;

            std::cout << "chars in range: " << chars_in_range << std::endl;

            for(std::size_t i = 0; i < chars_in_range; ++i)
            {
                std::shared_ptr<Character> character = std::shared_ptr<Character>(new Character());

                unsigned char char_buf = 0;
                packet >> character->name;
                packet >> character->map_id;
                packet >> character->x;
                packet >> character->y;
                packet >> char_buf; character->direction = (Direction)char_buf;
                packet >> char_buf; character->gender = (Gender)char_buf;

                map.characters.push_back(character);
            }

            client.character = map.GetCharacter(character.name);

            std::cout << "Char pos: " << character.x << "x" << character.y << std::endl;
        }

        std::cout << message << std::endl;
    }

    void Face(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        std::string name = "";
        unsigned char direction = 0;

        packet >> name;
        packet >> direction;

        Character *character = Map().GetCharacter(name);
        character->direction = (Direction)direction;
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
}
