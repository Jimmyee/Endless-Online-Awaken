// Endless Online Awaken

#include "packet_handler.hpp"

PacketHandler::Handler::Handler()
{
    this->packet_id = PacketID::Init;
    this->func = NULL;
}

PacketHandler::Handler::Handler(PacketID packet_id, HandlerFunction func, std::array<intptr_t, 4> data_ptr)
{
    this->packet_id = packet_id;
    this->func = func;
    this->data_ptr = data_ptr;
}

void PacketHandler::Handler::Execute(sf::Packet &packet)
{
    if(this->func != NULL) this->func(packet, this->data_ptr);
    else throw std::runtime_error("Network: Unknown data packet detected.");
}

void PacketHandler::Register(PacketID id, HandlerFunction func, std::array<intptr_t, 4> data_ptr)
{
    Handler handler(id, func, data_ptr);
    this->handlers.insert(std::pair<PacketID, Handler>(id, handler));
}

void PacketHandler::Unregister(PacketID id)
{
    auto it = this->handlers.find(id);

    if(it != this->handlers.end())
    {
        this->handlers.erase(it);
    }
}

void PacketHandler::Clear()
{
    this->handlers.clear();
}

void PacketHandler::Execute(sf::Packet &packet)
{
    unsigned short id;
    PacketID packet_id;

    packet >> id;
    packet_id = static_cast<PacketID>(id);

    auto it = this->handlers.find(packet_id);
    if(it != this->handlers.end())
    {
        this->handlers[packet_id].Execute(packet);
    }
}
