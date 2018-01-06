// Endless Online Awaken

#include "h_character.hpp"

#include "file_data.hpp"
#include "../server.hpp"
#include "../const/entity.hpp"
#include "../client.hpp"
#include "../database.hpp"
#include "../map_handler.hpp"
#include "../util.hpp"

#include <iostream>
#include <memory>

struct CreateRequest
{
    std::string name;
    bool found;
    int slots_used;

    CreateRequest() : name(""), found(false), slots_used(0) { }
};

static int lookup_character(void *data, int argc, char **argv, char **col_name)
{
    for(int i = 0; i < argc; i++)
    {
        std::cout << col_name[i] << ", " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }

    CreateRequest *request = (CreateRequest *)data;
    std::string name = request->name;

    if(argc == 1)
    {
        if(argv[0] == name)
        {
            request->found = true;
        }
    }

    return 0;
}

static int lookup_free_slot(void *data, int argc, char **argv, char **col_name)
{
    CreateRequest *request = (CreateRequest *)data;

    for(int i = 0; i < argc; i++)
    {
        std::cout << col_name[i] << ", " << (argv[i] ? argv[i] : "NULL") << std::endl;

        std::string column = col_name[i];

        if(column == "username") request->slots_used++;
    }

    if(request->slots_used >= 3)
    {
        request->found = false;
    }

    return 0;
}

static bool validate_string(std::string str)
{
    for(std::size_t i = 0; i < str.size(); ++i)
    {
        bool valid = false;

        if((str[i] >= 48 && str[i] <= 57) || (str[i] >= 65 && str[i] <= 90) || (str[i] >= 97 && str[i] <= 122))\
            valid = true;

        if(!valid) return false;
    }

    return true;
}

namespace PacketHandlers::HCharacter
{

    void Main(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        unsigned char sub_id = 0;
        packet >> sub_id;

        if(sub_id == 1) // create character
        {
            Create(packet, data_ptr);
        }
        if(sub_id == 2) // delete character
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
        if(sub_id == 5) // change direction
        {
            Talk(packet, data_ptr);
        }
        if(sub_id == 6) // chat
        {
            Face(packet, data_ptr);
        }
        if(sub_id == 7) // walk
        {
            Walk(packet, data_ptr);
        }
        if(sub_id == 8) // get character in range by name
        {
            GetInRange(packet, data_ptr);
        }
    }

    void Create(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        std::string name = "";
        unsigned char gender = 0;
        Client *client = (Client *)data_ptr[0];
        Database database;

        packet >> name;
        packet >> gender;

        std::unique_ptr<CreateRequest> request = std::make_unique<CreateRequest>();
        request->name = name;

        std::string sql_query = "SELECT * FROM characters WHERE username='" + client->username + "';";

        std::cout << sql_query << std::endl;

        bool valid_str = validate_string(name);

        if(valid_str)
            database.Execute(sql_query.c_str(), lookup_free_slot, request.get());

        unsigned char answer = 0;
        std::string message = "";

        if(request->slots_used < 3 && valid_str)
        {
            sql_query = "SELECT name FROM characters WHERE name='" + name + "';";

            std::cout << sql_query << std::endl;

            database.Execute(sql_query.c_str(), lookup_character, request.get());

            if(!request->found)
            {
                sql_query = "INSERT INTO characters VALUES ('" + client->username + "', '" + name + "', " \
                + "1, 1, 1, 1, " + std::to_string(gender) + ");";

                std::cout << sql_query << std::endl;

                int ret = database.Execute(sql_query.c_str(), 0, 0);

                if(ret == 0)
                {
                    std::shared_ptr<Character> character = std::shared_ptr<Character>(new Character());
                    character->name = name;
                    character->map_id = 1;
                    character->x = 1;
                    character->y = 1;
                    character->gender = (Gender)gender;
                    client->characters.push_back(character);
                    answer = 1;
                    message = "Character created.";
                }
            }
            else message = "Character of given name already exists.";
        }
        else
        {
            std::string errormsg = "";

            if(!valid_str) errormsg = "Please use alphanumeric characters only";
            else errormsg = "No free character slots";

            message = "Could not create character. -[" + errormsg + "]-";
        }

        std::cout << message << std::endl;

        sf::Packet reply;

        reply << (unsigned short)PacketID::Character;
        reply << (unsigned char)1; // character creation
        reply << answer;
        reply << message;
        if(answer == 1) // character created
        {
            std::size_t characters = client->characters.size();

            reply << characters;
            for(std::size_t i = 0; i < characters; ++i)
            {
                reply << client->characters[i]->name;
                unsigned char gender = (unsigned char)client->characters[i]->gender;
                reply << gender;
            }
        }

        client->Send(reply);
    }

    void Delete(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        Client *client = (Client *)data_ptr[0];
        Database database;
        std::string name = "";

        packet >> name;

        unsigned char answer = 0;
        std::string message = "";

        if(client->GetCharacter(name)->name != "")
        {
            std::string sql_query = "DELETE FROM characters WHERE name='" + name + "';";

            std::cout << sql_query << std::endl;

            int ret = database.Execute(sql_query.c_str(), 0, 0);

            if(ret == 0)
            {
                client->RemoveCharacter(name);
                answer = 1;
                message = "Character deleted.";
            }
            else
            {
                message = "Could not delete character of given name.";
            }
        }
        else
        {
            message = "Could not delete character of given name.";
        }

        std::cout << message << std::endl;

        sf::Packet reply;

        reply << (unsigned short)PacketID::Character;
        reply << (unsigned char)2; // delete character
        reply << answer;
        reply << message;
        if(answer == 1) reply << name;

        client->Send(reply);
    }

    void List(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        Client *client = (Client*)data_ptr[0];

        sf::Packet reply;

        reply << (unsigned short)PacketID::Character;
        reply << (unsigned char)3; // character list
        std::size_t characters = client->characters.size();
        reply << characters;
        for(std::size_t i = 0; i < characters; ++i)
        {
            reply << client->characters[i]->name;
            unsigned char gender = (unsigned char)client->characters[i]->gender;
            reply << gender;
        }

        client->Send(reply);
    }

    void Select(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        Client *client = (Client *)data_ptr[0];
        std::string name = "";

        if(client->state != Client::State::Logged_in) return;

        packet >> name;

        unsigned char answer = 0;
        std::string message = "";
        Character *character = client->GetCharacter(name);

        if(character != 0)
        {
            Map *map = MapHandler().GetMap(character->map_id);
            map->characters.push_back(std::shared_ptr<Character>(new Character(*character)));
            client->selected_character = name;
            character = map->GetCharacter(name);
            client->map_id = character->map_id;

            std::cout << "welcome character name: " << character->name << std::endl;

            client->state = Client::State::Playing;
            client->packet_handler.Register(PacketID::FileData, PacketHandlers::FileData::Main, data_ptr);

            answer = 1;
            message = "Welcome.";

            sf::Packet reply;

            reply << (unsigned short)PacketID::Character;
            reply << (unsigned char)4; // sub id
            reply << answer;
            reply << message;

            reply << character->name;
            reply << character->map_id;
            /*reply << character->x;
            reply << character->y;
            reply << (unsigned char)character->direction;
            reply << (unsigned char)character->gender;*/

            std::vector<std::shared_ptr<Character>> chars_in_range;
            for(auto &it : map->characters)
            {
                int len = path_length(character->x, character->y, it->x, it->y);

                if(len < 13)
                {
                    chars_in_range.push_back(it);
                }
            }

            reply << chars_in_range.size();

            for(auto &it : chars_in_range)
            {
                reply << it->name;
                reply << it->map_id;
                reply << it->x;
                reply << it->y;
                reply << (unsigned char)it->direction;
                reply << (unsigned char)it->gender;
            }

            client->Send(reply);

            reply.clear();

            std::cout << "Character name: " << character->name << std::endl;

            reply << (unsigned short)PacketID::Map;
            reply << (unsigned char)1; // appear
            reply << character->name;
            reply << character->map_id;
            reply << character->x;
            reply << character->y;
            reply << (unsigned char)character->direction;
            reply << (unsigned char)character->gender;

            for(auto &it : chars_in_range)
            {
                if(it->name == character->name) continue;

                Client *char_client = Server().GetClientByChar(it->name);
                char_client->Send(reply);
            }

            character->chars_in_range = chars_in_range;

            std::cout << "Client state: " << (int)client->state << std::endl;
            std::cout << "Characters on the map: " << map->characters.size() << std::endl;

            reply.clear();

            reply << (unsigned short)PacketID::Character;
            reply << (unsigned char)4;
            reply << (unsigned char)2;
            reply << message;
            reply << map->revision;

            client->Send(reply);

        }
        else
        {
            message = "Could not enter the game.";

            sf::Packet reply;

            reply << (unsigned short)PacketID::Character;
            reply << (unsigned char)4; // sub id
            reply << answer;
            reply << message;

            client->Send(reply);
        }
    }

    void Talk(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        Client *client = (Client*)data_ptr[0];
        unsigned char channel = 0;
        std::string message = "";

        packet >> channel;
        packet >> message;

        if(message.length() > 255) return;
        if(client->state != Client::State::Playing) return;

        //temporary
        if(channel != 0) return;

        Map *map = MapHandler().GetMap(client->map_id);
        Character *character = map->GetCharacter(client->selected_character);

        std::cout << character->name << ": " << message << std::endl;

        std::vector<std::shared_ptr<Character>> chars_in_range;
        for(auto &it : map->characters)
        {
            if(it->name == character->name) continue;

            int len = path_length(character->x, character->y, it->x, it->y);

            if(len < 13)
            {
                chars_in_range.push_back(it);
            }
        }

        sf::Packet reply;

        reply << (unsigned short)PacketID::Character;
        reply << (unsigned char)5; // talk
        reply << (unsigned char)0; // public
        reply << character->name;
        reply << message;

        std::cout << "in range: " << chars_in_range.size() << std::endl;

        client->Send(reply);

        for(auto &it : chars_in_range)
        {
            Client *char_client = Server().GetClientByChar(it->name);

            std::cout << it->name << std::endl;
            if(!char_client)
            {
                std::cout << "character " << it->name << " not found" << std::endl;
                continue;
            }

            char_client->Send(reply);

            if(character->GetInRange(it->name) == 0)
            {
                sf::Packet reply_appear;

                reply_appear << (unsigned short)PacketID::Map;
                reply_appear << (unsigned char)1; // appear
                reply_appear << it->name;
                reply_appear << it->map_id;
                reply_appear << it->x;
                reply_appear << it->y;
                reply_appear << (unsigned char)it->direction;
                reply_appear << (unsigned char)it->gender;

                client->Send(reply_appear);
            }
        }

        character->chars_in_range = chars_in_range;
    }

    void Face(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        Client *client = (Client*)data_ptr[0];
        unsigned char direction = 0;

        packet >> direction;

        if(direction < 0 || direction > 3) return;
        if(client->state != Client::State::Playing) return;

        Map *map = MapHandler().GetMap(client->map_id);
        Character *character = map->GetCharacter(client->selected_character);

        character->direction = (Direction)direction;

        sf::Packet reply;

        reply << (unsigned short)PacketID::Character;
        reply << (unsigned char)6;
        reply << character->name;
        reply << (unsigned char)character->direction;

        std::cout << "characters on map: " << map->characters.size() << std::endl;

        std::vector<std::shared_ptr<Character>> chars_in_range;
        for(auto &it : map->characters)
        {
            if(it->name == character->name) continue;

            int len = path_length(character->x, character->y, it->x, it->y);

            if(len < 16)
            {
                chars_in_range.push_back(it);
            }
        }

        std::cout << "chars in range: " << chars_in_range.size() << std::endl;

        for(auto &it : chars_in_range)
        {
            Client *char_client = Server().GetClientByChar(it->name);
            char_client->Send(reply);

            if(character->GetInRange(it->name) == 0)
            {
                sf::Packet reply_appear;

                reply_appear << (unsigned short)PacketID::Map;
                reply_appear << (unsigned char)1; // appear
                reply_appear << it->name;
                reply_appear << it->map_id;
                reply_appear << it->x;
                reply_appear << it->y;
                reply_appear << (unsigned char)it->direction;
                reply_appear << (unsigned char)it->gender;

                client->Send(reply_appear);
            }

            std::cout << "SEND\n";
        }

        character->chars_in_range = chars_in_range;
    }

    void Walk(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        Client *client = (Client*)data_ptr[0];
        unsigned char direction = 0;

        packet >> direction;

        if(direction < 0 || direction > 3) return;
        if(client->state != Client::State::Playing) return;

        Map *map = MapHandler().GetMap(client->map_id);
        Character *character = map->GetCharacter(client->selected_character);

        character->direction = (Direction)direction;

        if(character->direction == Direction::Up && character->y > 0) character->y--;
        if(character->direction == Direction::Right && character->x < map->width - 1) character->x++;
        if(character->direction == Direction::Down && character->y < map->height - 1) character->y++;
        if(character->direction == Direction::Left && character->x > 0) character->x--;

        sf::Packet reply;

        reply << (unsigned short)PacketID::Character;
        reply << (unsigned char)7;
        reply << character->name;
        reply << (unsigned char)character->direction;
        reply << character->x;
        reply << character->y;

        std::cout << "characters on map: " << map->characters.size() << std::endl;

        std::vector<std::shared_ptr<Character>> chars_in_range;
        for(auto &it : map->characters)
        {
            if(it->name == character->name) continue;

            int len = path_length(character->x, character->y, it->x, it->y);

            if(len < 16)
            {
                chars_in_range.push_back(it);
            }
        }

        std::cout << "chars in range: " << chars_in_range.size() << std::endl;

        std::cout << "ELO\n";

        for(auto &it : chars_in_range)
        {
            Client *char_client = Server().GetClientByChar(it->name);

            std::cout << it->name << "\n";

            char_client->Send(reply);

            if(character->GetInRange(it->name) == 0)
            {
                sf::Packet reply_appear;

                reply_appear << (unsigned short)PacketID::Map;
                reply_appear << (unsigned char)1; // appear
                reply_appear << it->name;
                reply_appear << it->map_id;
                reply_appear << it->x;
                reply_appear << it->y;
                reply_appear << (unsigned char)it->direction;
                reply_appear << (unsigned char)it->gender;

                client->Send(reply_appear);
            }

            std::cout << "SEND\n";
        }

        character->chars_in_range = chars_in_range;
    }

    void GetInRange(sf::Packet packet, std::array<intptr_t, 4> data_ptr)
    {
        Client *client = (Client*)data_ptr[0];
        std::string name = "";

        packet >> name;

        Map *map = MapHandler().GetMap(client->map_id);
        Character *character = map->GetCharacter(name);

        if(character == 0) return;

        sf::Packet reply;

        reply << (unsigned short)PacketID::Map;
        reply << (unsigned char)1; // appear
        reply << character->name;
        reply << character->map_id;
        reply << character->x;
        reply << character->y;
        reply << (unsigned char)character->direction;
        reply << (unsigned char)character->gender;

        client->Send(reply);
    }
}
