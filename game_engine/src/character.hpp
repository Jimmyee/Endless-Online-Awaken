// Endless Online Awaken

#ifndef CHARACTER_HPP_INCLUDED
#define CHARACTER_HPP_INCLUDED

#include "const/entity.hpp"
#include "animation.hpp"

#include <string>

class Character
{
public:
    enum class AnimState
    {
        Stand,
        Sit,
        Walk
    };

public:
    std::string name;
    unsigned short map_id;
    unsigned short x;
    unsigned short y;
    Direction direction;
    Gender gender;

    AnimState anim_state;
    Animation animation;

    Character();
    Character(std::string name, unsigned short map_id, unsigned short x, unsigned short y);

    void Tick();
    void Render(int rx, int ry);

    void Talk(unsigned char channel, std::string message, std::string char_name = "");
    void Face(Direction direction);
    void Walk(Direction direction);
};

#endif // CHARACTER_HPP_INCLUDED
