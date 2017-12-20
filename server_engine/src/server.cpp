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
std::vector<std::shared_ptr<sf::TcpSocket>> Server::sockets;
std::vector<std::shared_ptr<Client>> Server::clients;

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
        this->listener.listen(port);
        this->listener.setBlocking(false);

        this->initialized_ = true;

        std::cout << "Listening on port " << port << "..." << std::endl;
    }
}

void Server::Tick()
{
    std::shared_ptr<sf::TcpSocket> socket = std::shared_ptr<sf::TcpSocket>(new sf::TcpSocket());

    sf::Socket::Status status = this->listener.accept(*socket);

    if(status == sf::Socket::Done)
    {
        std::array<intptr_t, 4> ptr;
        std::shared_ptr<Client> client = std::shared_ptr<Client>(new Client());
        //Client client;

        socket->setBlocking(false);
        client->socket = socket;
        this->sockets.push_back(socket);
        this->clients.push_back(client);

        ptr[0] = (intptr_t)this->clients[this->clients.size() - 1].get();
        client->packet_handler.Register(PacketID::Init, PacketHandlers::HInit::Main, ptr);

        std::cout << "New client accepted (" << this->clients.size() << ")" << std::endl;
    }
    else if(status == sf::Socket::Error)
    {
        throw std::runtime_error("Server: socket error.");
    }

    std::vector<std::pair<std::shared_ptr<Client>, int>> dead_clients;
    for(std::size_t i = 0; i < this->clients.size(); ++i)
    {
        this->clients[i]->Tick();
        if(!this->clients[i]->Connected())
        {
            Database database;
            if(this->clients[i]->state == Client::State::Logged_in)
            {
                for(std::size_t ii = 0; ii < this->clients[i]->characters.size(); ++ii)
                {
                    Character *chracter = this->clients[i]->characters[ii].get();

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
                std::cout << "SELECTED CHARACTER: " << this->clients[i]->selected_character << std::endl;

                for(std::size_t ii = 0; ii < this->clients[i]->characters.size(); ++ii)
                {
                    if(this->clients[i]->characters[ii]->name == this->clients[i]->selected_character) continue;

                    Character *character = this->clients[i]->characters[ii].get();

                    std::string sql_query = "UPDATE characters SET ";
                    sql_query += "map_id = " + std::to_string(character->map_id);
                    sql_query += ", x = " + std::to_string(character->x);
                    sql_query += ", y = " + std::to_string(character->y);
                    sql_query += ", direction = " + std::to_string((int)character->direction);
                    sql_query += ", gender = " + std::to_string((int)character->gender);

                    sql_query += " WHERE name = '" + character->name + "';";

                    std::cout << sql_query << std::endl;

                    database.Execute(sql_query.c_str(), 0, 0);

                    Map *map = MapHandler().GetMap(this->clients[i]->map_id);
                    map->DeleteCharacter(character->name);
                }

                Map *map = MapHandler().GetMap(this->clients[i]->map_id);
                Character *character = map->GetCharacter(this->clients[i]->selected_character);

                std::string sql_query = "UPDATE characters SET ";
                sql_query += "map_id = " + std::to_string(character->map_id);
                sql_query += ", x = " + std::to_string(character->x);
                sql_query += ", y = " + std::to_string(character->y);
                sql_query += ", direction = " + std::to_string((int)character->direction);
                sql_query += ", gender = " + std::to_string((int)character->gender);

                sql_query += " WHERE name = '" + character->name + "';";

                std::cout << sql_query << std::endl;

                database.Execute(sql_query.c_str(), 0, 0);

                map->DeleteCharacter(character->name);
            }

            int socket_index = -1;
            for(std::size_t iii = 0; iii < this->sockets.size(); ++iii)
            {
                if(this->sockets[iii] == this->clients[i]->socket)
                {
                    socket_index = iii;
                    break;
                }
            }

            if(socket_index != -1)
            {
                this->sockets.erase(this->sockets.begin() + socket_index);
                this->clients.erase(this->clients.begin() + i);
            }

            std::cout << "Client disconnected." << std::endl;
            break;
        }
    }

    /*int offset = 0;
    for(std::size_t i = 0; i < dead_clients.size(); ++i)
    {
        this->clients.erase(this->clients.begin() + std::get<1>(dead_clients[i]) + offset);
        offset--;
    }*/
}

Client *Server::GetClient(std::string char_name)
{
    for(std::size_t i = 0; i < this->clients.size(); ++i)
    {
        std::cout << "---looking for character..." << std::endl;

        if(this->clients[i]->selected_character == "") continue;

        Map *map = MapHandler().GetMap(this->clients[i]->map_id);
        Character *character = map->GetCharacter(this->clients[i]->selected_character);

        std::cout << character->name << "---" << std::endl;

        if(character->name == char_name)
        {
            return this->clients[i].get();
        }
    }

    return 0;
}
