// Endless Online Awaken

#include "login.hpp"

#include "h_character.hpp"
#include "ping.hpp"
#include "../client.hpp"
#include "../database.hpp"
#include "../server.hpp"

#include <iostream>
#include <memory>

struct LoginRequest
{
    std::string username;
    std::string password;
    bool valid;

    LoginRequest() : username(""), password(""), valid(false) { }
};

struct CharList
{
    std::vector<std::shared_ptr<Character>> characters;
};

static int validate_account(void *data, int argc, char **argv, char **col_name)
{
    LoginRequest *request = (LoginRequest *)data;
    std::string username = request->username;

    for(int i = 0; i < argc; i++)
    {
        std::cout << col_name[i] << ", " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }

    if(argc == 1)
    {
        if(argv[0] == username)
        {
            request->valid = true;
        }
    }

    return 0;
}

static int get_characters(void *data, int argc, char **argv, char **col_name)
{
    CharList *charlist = (CharList *)data;

    for(int i = 0; i < argc; i++)
    {
        std::cout << col_name[i] << ", " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }

    if(argc >= 5)
    {
        std::shared_ptr<Character> character = std::shared_ptr<Character>(new Character());
        character->username = argv[0];
        character->name = argv[1];
        character->map_id = std::atoi(argv[2]);
        character->x = std::atoi(argv[3]);
        character->y = std::atoi(argv[4]);
        character->direction = (Direction)std::atoi(argv[5]);
        character->gender = (Gender)std::atoi(argv[6]);
        character->speed = std::atoi(argv[7]);

        charlist->characters.push_back(character);
    }

    return 0;
}

static bool is_alphanumeric(std::string str)
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

namespace PacketHandlers::HLogin
{

void Main(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
{
    Client *client = (Client *)data_ptr[0];

    unsigned char sub_id = 0;
    packet >> sub_id;

    if(sub_id == 1) // login request
    {
        std::string username = "";
        std::string password = "";

        if(client->state != Client::State::Initialized) return;

        packet >> username;
        packet >> password;

        std::string sql_query = "SELECT username FROM accounts WHERE username='" \
        + username + "' AND password='" + password + "';";

        std::cout << sql_query << std::endl;

        Database database;
        std::unique_ptr<LoginRequest> request = std::make_unique<LoginRequest>();
        request->username = username;
        request->password = password;

        Client *client_logged = Server().GetClientByAcc(username);
        bool valid_str = is_alphanumeric(username) && is_alphanumeric(password);

        if(valid_str && client_logged == 0)
        {
            database.Execute(sql_query.c_str(), validate_account, request.get());
        }

        unsigned char answer = 0;
        std::string message = "";

        if(request->valid)
        {
            client->state = Client::State::Logged_in;
            client->username = username;
            client->packet_handler.Register(PacketID::Character, PacketHandlers::HCharacter::Main, data_ptr);
            answer = 1;
            message = "Logged in!";

            sql_query = "SELECT * FROM characters WHERE username='" + client->username + "';";
            std::unique_ptr<CharList> charlist = std::make_unique<CharList>();
            database.Execute(sql_query.c_str(), get_characters, charlist.get());

            client->characters = charlist->characters;
        }
        else
        {
            std::string errormsg = "";

            if(client_logged != 0) errormsg = "Already logged in";
            if(!valid_str) errormsg = "Please use alphanumeric characters only";

            message = "Could not login! -[" + errormsg + "]-";
        }

        std::cout << message << std::endl;

        sf::Packet reply;
        reply << (unsigned char)PacketID::Login;
        reply << (unsigned char)1; // sub id
        reply << answer;
        reply << message;
        if(request->valid)
        {
            std::size_t characters = client->characters.size();
            reply << characters;
            for(std::size_t i = 0; i < characters; ++i)
            {
                reply << client->characters[i]->name;
                reply << (unsigned char)client->characters[i]->gender;
            }
        }
        client->Send(reply);
    }
}

}
