// Endless Online Awaken

#include "client.hpp"

#include "handlers/init.hpp"

#include <iostream>

Client::Client()
{
    this->connected = true;
    this->state = State::Uninitialized;
    this->username = "";
    this->selected_character = "";
    this->map_id = 0;
}

void Client::Tick()
{
    sf::Packet packet;
    sf::Socket::Status status;

    if(!this->connected) return;

    if(!this->send_queue.empty())
    {
        status = this->socket->send(this->send_queue[0]);

        switch(status)
        {
        case sf::Socket::Status::Done:
            this->send_queue.erase(this->send_queue.begin());
            break;

        case sf::Socket::Status::NotReady:
            break;

        case sf::Socket::Status::Disconnected:
            this->connected = false;
            break;

        case sf::Socket::Status::Error:
            {
                this->connected = false;
                throw std::runtime_error("Client socket: error while sending data.");
            }
            break;

        default:
            break;
        }
    }

    status = this->socket->receive(packet);

    switch(status)
    {
    case sf::Socket::Status::Done:
        this->recv_queue.push_back(packet);
        break;

    case sf::Socket::Status::NotReady:
        break;

    case sf::Socket::Status::Disconnected:
        this->connected = false;
        break;

    case sf::Socket::Status::Error:
        {
            throw std::runtime_error("Client socket: error while receiving data.");
        }
        break;

    default:
        break;
    }

    if(!this->recv_queue.empty())
    {
        packet = this->Recv();
        this->packet_handler.Execute(packet);
    }
}

bool Client::Connected()
{
    return this->connected;
}

void Client::Send(sf::Packet packet)
{
    this->send_queue.push_back(packet);
}

sf::Packet Client::Recv()
{
    sf::Packet packet;

    if(!this->recv_queue.empty())
    {
        packet = this->recv_queue[0];
        this->recv_queue.erase(this->recv_queue.begin());
    }

    return packet;
}

Character *Client::GetCharacter(std::string name)
{
    for(auto &it : this->characters)
    {
        if(it->name == name) return it.get();
    }

    return 0;
}

void Client::RemoveCharacter(std::string name)
{
    for(std::size_t i = 0; i < this->characters.size(); ++i)
    {
        if(this->characters[i]->name == name)
        {
            this->characters.erase(this->characters.begin() + i);
            return;
        }
    }
}
