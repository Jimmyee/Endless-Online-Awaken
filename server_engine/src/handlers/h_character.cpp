// Endless Online Awaken

#include "h_character.hpp"

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


namespace PacketHandlers::HCharacter
{

    void Main(sf::Packet packet, std::array<intptr_t, 4> data_ptr)
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
        if(sub_id == 5)
        {
            Face(packet, data_ptr);
        }
        if(sub_id == 6)
        {
            Talk(packet, data_ptr);
        }
    }

    void Create(sf::Packet packet, std::array<intptr_t, 4> data_ptr)
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

        database.Execute(sql_query.c_str(), lookup_free_slot, request.get());

        unsigned char answer = 0;
        std::string message = "";

        if(request->slots_used < 3)
        {
            sql_query = "SELECT name FROM characters WHERE name='" + name + "';";

            std::cout << sql_query << std::endl;

            database.Execute(sql_query.c_str(), lookup_character, request.get());

            if(!request->found)
            {
                sql_query = "INSERT INTO characters VALUES ('" + client->username + "', '" + name + "', " \
                + "1, 1, 1, " + std::to_string(gender) + ");";

                std::cout << sql_query << std::endl;

                int ret = database.Execute(sql_query.c_str(), 0, 0);

                if(ret == 0)
                {
                    Character character;
                    character.name = name;
                    character.gender = (Gender)gender;
                    client->characters.push_back(character);
                    answer = 1;
                    message = "Character created.";
                }
            }
            else message = "Character of given name already exists.";
        }
        else message = "No free character slots.";

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
                reply << client->characters[i].name;
                unsigned char gender = (unsigned char)client->characters[i].gender;
                reply << gender;
            }
        }
        client->Send(reply);
    }

    void Delete(sf::Packet packet, std::array<intptr_t, 4> data_ptr)
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

    void List(sf::Packet packet, std::array<intptr_t, 4> data_ptr)
    {
        Client *client = (Client*)data_ptr[0];
        sf::Packet reply;

        reply << (unsigned short)PacketID::Character;
        reply << (unsigned char)3; // character list
        std::size_t characters = client->characters.size();
        reply << characters;
        for(std::size_t i = 0; i < characters; ++i)
        {
            reply << client->characters[i].name;
            unsigned char gender = (unsigned char)client->characters[i].gender;
            reply << gender;
        }

        client->Send(reply);
    }

    void Select(sf::Packet packet, std::array<intptr_t, 4> data_ptr)
    {
        Client *client = (Client *)data_ptr[0];
        std::string name = "";

        if(client->state != Client::State::Logged_in) return;

        packet >> name;

        unsigned char answer = 0;
        std::string message = "";
        Map *map = 0;
        Character *character = client->GetCharacter(name);
        if(character != 0)
        {
            MapHandler map_handler;

            map = map_handler.GetMap(character->map_id);
            map->characters.push_back(*character);
            client->character = map->GetCharacter(name);

            client->state = Client::State::Playing;

            answer = 1;
            message = "Welcome.";
        }
        else message = "Could not enter the game.";

        sf::Packet reply;
        reply << (unsigned short)PacketID::Character;
        reply << (unsigned char)4; // sub id
        reply << answer;
        reply << message;
        if(answer == 1)
        {
            reply << client->character->name;
            reply << client->character->map_id;
            reply << client->character->x;
            reply << client->character->y;
            reply << (unsigned char)client->character->direction;

            std::vector<Character> chars_in_range;
            for(auto &it : map->characters)
            {
                if(it.name == client->character->name) continue;

                int len = path_length(client->character->x, client->character->y, it.x, it.y);

                if(len < 13)
                {
                    chars_in_range.push_back(it);
                }
            }

            reply << chars_in_range.size();

            for(auto &it : chars_in_range)
            {
                reply << it.name;
                reply << it.map_id;
                reply << it.x;
                reply << it.y;
                reply << (unsigned char)it.direction;
                reply << (unsigned char)it.gender;
            }
        }
        client->Send(reply);
    }

    void Face(sf::Packet packet, std::array<intptr_t, 4> data_ptr)
    {
        Client *client = (Client*)data_ptr[0];
        unsigned char direction = 0;

        packet >> direction;

        if(direction < 0 || direction > 3) return;
        if(client->state != Client::State::Playing) return;

        client->character->direction = (Direction)direction;

        std::cout << "face" << std::endl;

        Map *map = MapHandler().GetMap(client->character->map_id);

        sf::Packet reply;
        reply << (unsigned short)PacketID::Character;
        reply << (unsigned char)5;
        reply << client->character->name;
        reply << (unsigned char)client->character->direction;

        std::vector<Character> chars_in_range;
        for(auto &it : map->characters)
        {
            if(it.name == client->character->name) continue;

            int len = path_length(client->character->x, client->character->y, it.x, it.y);

            if(len < 13)
            {
                chars_in_range.push_back(it);
            }
        }

        for(auto &it : chars_in_range)
        {
            Client *char_client = Server().GetClient(it.name);
            char_client->Send(reply);
        }
    }

    void Talk(sf::Packet packet, std::array<intptr_t, 4> data_ptr)
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

        Map *map = MapHandler().GetMap(client->character->map_id);

        std::vector<Character> chars_in_range;
        for(auto &it : map->characters)
        {
            int len = path_length(client->character->x, client->character->y, it.x, it.y);

            if(len < 13)
            {
                chars_in_range.push_back(it);
            }
        }

        sf::Packet reply;
        reply << (unsigned short)PacketID::Character;
        reply << (unsigned char)6; // talk
        reply << (unsigned char)0; // public
        reply << client->character->name;
        reply << message;

        for(auto &it : chars_in_range)
        {
            //if(it.name == client->character->name) continue;

            Client *char_client = Server().GetClient(it.name);

            char_client->Send(reply);
        }
    }
}
