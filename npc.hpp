// Endless Online Awaken v0.0.1

#ifndef NPC_HPP_INCLUDED
#define NPC_HPP_INCLUDED

#include "const/character.hpp"
#include "eodata.hpp"

#include <memory>

using std::shared_ptr;

class NPC
{
public:
    unsigned char gameworld_index;
    short id;
    Direction direction;
    unsigned char x, y;
    shared_ptr<ENF_Data> data;

    NPC();
};

#endif // NPC_HPP_INCLUDED
