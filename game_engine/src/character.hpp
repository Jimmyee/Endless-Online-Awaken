// Endless Online Awaken

#ifndef CHARACTER_HPP_INCLUDED
#define CHARACTER_HPP_INCLUDED

#include "const/entity.hpp"

#include <string>

class Character
{
public:
    std::string name;
    unsigned int id;
    unsigned short map_id;
    unsigned short x;
    unsigned short y;
    Direction direction;
    Gender gender;

    Character();
    Character(std::string name, unsigned int id, unsigned short map_id, unsigned short x, unsigned short y);

    void Render(int rx, int ry);

    void Face(Direction direction);
    void Talk(unsigned char channel, std::string message, std::string char_name = "");
};

#endif // CHARACTER_HPP_INCLUDED
