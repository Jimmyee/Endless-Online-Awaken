// Endless Online Awaken

#ifndef CHARACTER_HPP_INCLUDED
#define CHARACTER_HPP_INCLUDED

#include "const/entity.hpp"

#include <string>
#include <vector>
#include <memory>

class Character
{
public:
    std::string username;
    std::string name;
    unsigned short map_id;
    unsigned short x;
    unsigned short y;
    Direction direction;
    Gender gender;

    std::vector<std::shared_ptr<Character>> chars_in_range;

    Character();
    Character(std::string username, std::string name, unsigned short map_id, unsigned short x, unsigned short y);

    void Render(int rx, int ry);

    Character *GetInRange(std::string name);
    void DeleteInRange(std::string name);
};

#endif // CHARACTER_HPP_INCLUDED
