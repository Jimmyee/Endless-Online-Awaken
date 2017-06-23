// Endless Online Awaken v0.0.1

#ifndef CHARACTER_HPP_INCLUDED
#define CHARACTER_HPP_INCLUDED

#include "const/character.hpp"

#include <string>

class Character
{
public:

    std::string name;
    unsigned int id;
    short gameworld_id;

    std::string title;
    std::string guild;
    std::string guild_rank;
    std::string guild_tag;

    unsigned char clas;

    unsigned char admin_level;
    unsigned char level;

    int exp;
    int usage;

    short hp;
    short max_hp;
    short tp;
    short max_tp;
    short max_sp;

    short stat_points;
    short skill_points;

    short karma;

    short min_dam;
    short max_dam;

    short accuracy;
    short evade;
    short armor;

    short str;
    short wis;
    short intl;
    short agi;
    short con;
    short cha;

    short weight;
    short max_weight;

    short x;
    short y;
    Direction direction;
    Gender gender;
    unsigned char hair_style;
    unsigned char hair_color;
    Skin race;

    SitState sitting;
    unsigned char visibility;

    Character();
};

#endif // CHARACTER_HPP_INCLUDED
