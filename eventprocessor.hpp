#ifndef EVENTPROCESSOR_HPP_INCLUDED
#define EVENTPROCESSOR_HPP_INCLUDED

#include "config.hpp"

#include <SFML/System.hpp>
#include <vector>
#include <memory>

struct EventProcessor
{
    struct Trade
    {
        short victim_gameworld_id;
        std::vector<std::pair<short, int>> player_items;
        std::vector<std::pair<short, int>> victim_items;
        bool player_accepted;
        bool victim_accepted;

        Trade(short victim_gameworld_id_) { victim_gameworld_id = victim_gameworld_id_; player_accepted = false; victim_accepted = false; }
    };

    std::shared_ptr<Trade> trade;

    EventProcessor();

    void Process();
};

#endif // EVENTPROCESSOR_HPP_INCLUDED
