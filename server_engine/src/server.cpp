// Endless Online Awaken

#include "server.hpp"
#include "handlers/init.hpp"
#include "const/packet.hpp"
#include "map_handler.hpp"
#include "database.hpp"

#include <iostream>
#include <stdexcept>

bool Server::initialized_ = false;
sf::TcpListener Server::listener;
std::vector<std::unique_ptr<Client>> Server::clients;

Server::Server()
{
    if(!this->initialized_)
    {
        this->initialized_ = true;
    }
}

Server::Server(unsigned short port)
{
    if(!this->initialized_)
    {
        this->initialized_ = true;

        this->listener.listen(port);
        this->listener.setBlocking(false);

        std::cout << "Listening on port " << port << "..." << std::endl;
    }
}

void Server::Tick()
{
    std::unique_ptr<Client> client(new Client());

    sf::Socket::Status status = this->listener.accept(client->socket);

    if(status == sf::Socket::Done)
    {
        std::array<intptr_t, 4> ptr;
        ptr[0] = (intptr_t)client.get();

        client->packet_handler.Register(PacketID::Init, PacketHandlers::HInit::Main, ptr);
        this->clients.push_back(std::move(client));

        std::cout << "New client accepted (" << this->clients.size() << ")" << std::endl;
    }
    else if(status == sf::Socket::Error)
    {
        throw std::runtime_error("Server: socket error.");
    }

    Database database;
    for(std::size_t i = 0; i < this->clients.size(); ++i)
    {
        this->clients[i]->Tick();
        if(!this->clients[i]->Connected())
        {
            if(this->clients[i]->state == Client::State::Logged_in)
            {
                for(std::size_t ii = 0; ii < this->clients[i]->characters.size(); ++ii)
                {
                    Character *chracter = &this->clients[i]->characters[ii];

                    std::string sql_query = "UPDATE characters SET ";
                    sql_query += "map_id = " + std::to_string(chracter->map_id);
                    sql_query += ", x = " + std::to_string(chracter->x);
                    sql_query += ", y = " + std::to_string(chracter->y);
                    sql_query += ", direction = " + std::to_string((int)chracter->direction);
                    sql_query += ", gender = " + std::to_string((int)chracter->gender);

                    sql_query += " WHERE name = '" + chracter->name + "';";

                    std::cout << sql_query << std::endl;

                    database.Execute(sql_query.c_str(), 0, 0);
                }
            }
            if(this->clients[i]->state == Client::State::Playing)
            {
                for(std::size_t ii = 0; ii < this->clients[i]->characters.size(); ++ii)
                {
                    if(this->clients[i]->characters[ii].name == this->clients[i]->character->name) continue;

                    Character *chracter = &this->clients[i]->characters[ii];

                    std::string sql_query = "UPDATE characters SET ";
                    sql_query += "map_id = " + std::to_string(chracter->map_id);
                    sql_query += ", x = " + std::to_string(chracter->x);
                    sql_query += ", y = " + std::to_string(chracter->y);
                    sql_query += ", direction = " + std::to_string((int)chracter->direction);
                    sql_query += ", gender = " + std::to_string((int)chracter->gender);

                    sql_query += " WHERE name = '" + chracter->name + "';";

                    std::cout << sql_query << std::endl;

                    database.Execute(sql_query.c_str(), 0, 0);
                }

                Character *chracter = this->clients[i]->character;

                std::string sql_query = "UPDATE characters SET ";
                sql_query += "map_id = " + std::to_string(chracter->map_id);
                sql_query += ", x = " + std::to_string(chracter->x);
                sql_query += ", y = " + std::to_string(chracter->y);
                sql_query += ", direction = " + std::to_string((int)chracter->direction);
                sql_query += ", gender = " + std::to_string((int)chracter->gender);

                sql_query += " WHERE name = '" + chracter->name + "';";

                std::cout << sql_query << std::endl;

                database.Execute(sql_query.c_str(), 0, 0);

                Map *map = MapHandler().GetMap(this->clients[i]->character->map_id);
                map->DeleteCharacter(this->clients[i]->character->name);
            }

            this->clients.erase(this->clients.begin() + i);

            std::cout << "Client disconnected." << std::endl;
            break;
        }
    }
}

Client *Server::GetClient(std::string char_name)
{
    for(std::size_t i = 0; i < this->clients.size(); ++i)
    {
        if(!this->clients[i]->character) continue;

        if(this->clients[i]->character->name == char_name)
        {
            return this->clients[i].get();
        }
    }

    return 0;
}
