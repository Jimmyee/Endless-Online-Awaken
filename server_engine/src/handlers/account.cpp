// Endless Online Awaken

#include "account.hpp"

#include "../client.hpp"
#include "../database.hpp"
#include "../const/packet.hpp"

#include <iostream>

struct CreateRequest
{
    std::string username;
    bool found;

    CreateRequest() : username(""), found(false) { }
};

static int lookup_account(void *data, int argc, char **argv, char **col_name)
{
    for(int i = 0; i<argc; i++)
    {
        std::cout << col_name[i] << ", " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }

    CreateRequest *request = (CreateRequest *)data;
    std::string username = request->username;

    if(argc == 1)
    {
        if(argv[0] == username)
        {
            request->found = true;
        }
    }

    return 0;
}

namespace PacketHandlers::HAccount
{

void Main(sf::Packet packet, std::array<intptr_t, 4> data_ptr)
{
    Client *client = (Client *)data_ptr[0];

    if(client->state != Client::State::Initialized) return;

    unsigned char sub_id = 0;

    packet >> sub_id;

    if(sub_id == 1) // create an account
    {
        Create(packet, data_ptr);
    }
    if(sub_id == 2) // delete an account
    {
        Delete(packet, data_ptr);
    }
}

void Create(sf::Packet packet, std::array<intptr_t, 4> data_ptr)
{
    Client *client = (Client *)data_ptr[0];
    std::string username, password, real_name, location, email;

    packet >> username;
    packet >> password;
    packet >> real_name;
    packet >> location;
    packet >> email;

    std::string sql_query = "SELECT username FROM accounts WHERE username='" + username + "';";

    std::cout << sql_query << std::endl;

    Database database;
    std::unique_ptr<CreateRequest> request = std::make_unique<CreateRequest>();
    request->username = username;
    int ret = database.Execute(sql_query.c_str(), lookup_account, request.get());

    unsigned char answer = 0;
    std::string message = "";

    if(!request->found)
    {
        sql_query = "INSERT INTO accounts VALUES ('" + username + "', '" + password + "', '" + real_name + "', '" \
        + location + "', '" + email + "');";

        ret = database.Execute(sql_query.c_str(), 0, 0);

        if(ret == 0)
        {
            message = "Account created!";

            answer = 1;
        }
    }
    else message = "Could not create account of given username.";

    std::cout << message << std::endl;

    sf::Packet reply;
    reply << (unsigned short)PacketID::Account;
    reply << (unsigned char)1; // sub_id
    reply << answer;
    reply << message;
    client->Send(reply);
}

void Delete(sf::Packet packet, std::array<intptr_t, 4> data_ptr)
{

}

}
