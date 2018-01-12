// Endless Online Awaken

#ifndef NPC_HPP_INCLUDED
#define NPC_HPP_INCLUDED

#include "const/entity.hpp"
#include "animation.hpp"

#include <string>

class NPC
{
public:
    enum class AnimState
    {
        Stand,
        Sit,
        Walk
    };

public:
    unsigned int id;
    unsigned int index;
    std::string name;
    unsigned int map_id;
    unsigned short x;
    unsigned short y;
    Direction direction;
    unsigned char speed;

    AnimState anim_state;
    Animation animation;

    int frame_counter;

    NPC();
    NPC(unsigned int id, unsigned int index, unsigned int map_id, unsigned short x, unsigned short y);

    void Tick();

    void Talk(std::string message);
    void Face(Direction direction);
    void Walk(Direction direction);
};

#endif // NPC_HPP_INCLUDED
