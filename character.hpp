// Endless Online Awaken v0.0.1

#ifndef CHARACTER_HPP_INCLUDED
#define CHARACTER_HPP_INCLUDED

#include "const/character.hpp"

#include <SFML/Graphics.hpp>
#include <string>

class Animation
{
public:
    int frames;
    int delay;
    int current_frame;
    sf::Clock clock;
    bool play;

    Animation();
    Animation(int frames, int delay);
    int Process();
    void Play();
};

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

    AdminLevel admin_level;
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

    Animation animation;
    int xoff;
    int yoff;

    Character();
    void Process();
    void Draw(int x, int y);
};

#endif // CHARACTER_HPP_INCLUDED
