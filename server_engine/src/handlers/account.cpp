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

static bool validate_string_email(std::string str)
{
    for(std::size_t i = 0; i < str.size(); ++i)
    {
        bool valid = false;

        if((str[i] >= 48 && str[i] <= 57) || (str[i] >= 65 && str[i] <= 90) || (str[i] >= 97 && str[i] <= 122) ||
           str[i] == '@' || str[i] == '.')
            valid = true;

        if(!valid) return false;
    }

    return true;
}

namespace PacketHandlers::HAccount
{

void Main(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
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

void Create(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
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

    bool valid_str = true;

    if(!validate_string(username)) valid_str = false;
    if(!validate_string(password)) valid_str = false;
    if(!validate_string(real_name)) valid_str = false;
    if(!validate_string(location)) valid_str = false;
    if(!validate_string_email(email)) valid_str = false;

    int ret = 0;

    if(valid_str)
        ret = database.Execute(sql_query.c_str(), lookup_account, request.get());

    unsigned char answer = 0;
    std::string message = "";

    if(!request->found && valid_str)
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
    else
    {
        std::string errormsg = "";

        if(!valid_str) errormsg = "Please use alphanumeric characters only";
        else errormsg = "Account already exists";

        message = "Could not create account of given username. -[" + errormsg + "]-";
    }

    std::cout << message << std::endl;

    sf::Packet reply;
    reply << (unsigned short)PacketID::Account;
    reply << (unsigned char)1; // sub_id
    reply << answer;
    reply << message;
    client->Send(reply);
}

void Delete(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
{

}

}
