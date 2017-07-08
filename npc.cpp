#include "npc.hpp"

NPC::NPC()
{
    this->gameworld_index = 0;
    this->direction = static_cast<Direction>(0);
    this->x = 0;
    this->y = 0;
}
