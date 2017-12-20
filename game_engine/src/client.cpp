// Endless Online Awaken

#include "client.hpp"
#include "handlers/init.hpp"

#include <iostream>

bool Client::initialized_ = false;
bool Client::connected;
std::vector<sf::Packet> Client::recv_queue;
std::vector<sf::Packet> Client::send_queue;

std::unique_ptr<sf::TcpSocket> Client::socket;
PacketHandler Client::packet_handler;
Client::State Client::state;

std::vector<std::shared_ptr<Character>> Client::characters;
Character *Client::character;

Client::Client()
{
    if(!this->initialized_)
    {
        this->connected = false;
        this->state = State::Uninitialized;
        this->character = 0;

        this->initialized_ = true;
    }
}

bool Client::Connect(std::string address, unsigned short port)
{
    sf::Socket::Status status;

    if(this->connected) return true;

    std::cout << "Client socket: Connecting to " << address << ":" << port << std::endl;
    this->socket.reset(new sf::TcpSocket());
    status = this->socket->connect(address, port);

    if(status != sf::Socket::Done)
    {
        std::cout << "Client socket: can't connect." << std::endl;
        return false;
    }

    this->socket->setBlocking(false);
    this->connected = true;

    std::array<intptr_t, 4> ptr;
    ptr[0] = (intptr_t)this;

    this->packet_handler.Register(PacketID::Init, PacketHandlers::HInit::Main, ptr);

    this->characters.clear();

    std::cout << "Client socket: Connected" << std::endl;

    this->Init();

    return true;
}

void Client::Disconnect()
{
    this->socket->disconnect();
    this->connected = false;
    this->state = State::Uninitialized;
    this->characters.clear();
    this->character = 0;
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

void Client::Send(sf::Packet &packet)
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
    for(std::size_t i = 0; i < this->characters.size(); ++i)
    {
        if(this->characters[i]->name == name) return this->characters[i].get();
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

void Client::Init()
{
    sf::Packet packet;

    packet << (unsigned short)PacketID::Init;

    this->Send(packet);
}

void Client::Login(std::string username, std::string password)
{
    sf::Packet packet;

    packet << (unsigned short)PacketID::Login;
    packet << (unsigned char)1; // sub id = login request
    packet << username;
    packet << password;

    this->Send(packet);
}

void Client::CreateAccount(std::array<std::string, 5> input_data)
{
    sf::Packet packet;

    packet << (unsigned short)PacketID::Account;
    packet << (unsigned char)1; // sub id
    for(int i = 0; i < 5; ++i)
    {
        packet << input_data[i];
    }

    this->Send(packet);
}

void Client::CreateCharacter(std::string name, Gender gender)
{
    sf::Packet packet;

    packet << (unsigned short)PacketID::Character;
    packet << (unsigned char)1; // sub id - create
    packet << name;
    packet << (unsigned char)gender;

    this->Send(packet);
}

void Client::DeleteCharacter(std::string name)
{
    sf::Packet packet;

    packet << (unsigned short)PacketID::Character;
    packet << (unsigned char)2; // sub id - delete
    packet << name;

    this->Send(packet);
}

void Client::CharacterList()
{
    sf::Packet packet;

    packet << (unsigned short)PacketID::Character;
    packet << (unsigned char)3; // sub id - character list

    this->Send(packet);
}

void Client::SelectCharacter(std::string name)
{
    sf::Packet packet;

    packet << (unsigned short)PacketID::Character;
    packet << (unsigned char)4; // sub id - select
    packet << name;

    this->Send(packet);
}
