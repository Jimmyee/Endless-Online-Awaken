// Endless Online Awaken

#ifndef CHARACTER_HPP_INCLUDED
#define CHARACTER_HPP_INCLUDED

#include <SFML/System.hpp>
#include "const/entity.hpp"
#include "npc.hpp"

#include <string>
#include <vector>
#include <map>
#include <memory>

class Character
{
public:
    enum class Action
    {
        Face,
        Walk
    };

public:
    std::string username;
    std::string name;
    unsigned int map_id;
    unsigned short x;
    unsigned short y;
    Direction direction;
    Gender gender;
    unsigned char speed;

    std::vector<std::shared_ptr<Character>> chars_in_range;
    std::vector<std::shared_ptr<NPC>> npcs_in_range;

    std::map<Action, sf::Clock> action_clocks;

    Character();
    Character(std::string username, std::string name, unsigned int map_id, unsigned short x, unsigned short y);

    void Tick();
    void Render(int rx, int ry);

    Character *GetInRange(std::string name);
    void DeleteInRange(std::string name);
};

#endif // CHARACTER_HPP_INCLUDED
