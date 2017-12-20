// Endless Online Awaken

#ifndef PACKET_HANDLER_HPP_INCLUDED
#define PACKET_HANDLER_HPP_INCLUDED

#include "const/packet.hpp"

#include <SFML/Network.hpp>
#include <functional>

typedef std::function<void(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)> HandlerFunction;

class PacketHandler
{
public:
    struct Handler
    {
        PacketID packet_id;
        HandlerFunction func;
        std::array<intptr_t, 4> data_ptr;

        Handler();
        Handler(PacketID packet_id, HandlerFunction func, std::array<intptr_t, 4> data_ptr);
        void Execute(sf::Packet &packet);
    };

public:
    std::map<PacketID, Handler> handlers;

    void Register(PacketID id, HandlerFunction func, std::array<intptr_t, 4> data_ptr);
    void Unregister(PacketID id);
    void Clear();
    void Execute(sf::Packet &packet);
};

#endif // PACKET_HANDLER_HPP_INCLUDED
