// Endless Online Awaken v0.0.1

#include "character.hpp"

Character::Character()
{
    this->id = 0;
    this->gameworld_id = 0;
    this->clas = 0;
    this->admin_level = 0;
    this->level = 0;
    this->exp = 0;
    this->usage = 0;
    this->hp = 0;
    this->max_hp = 0;
    this->tp = 0;
    this->max_tp = 0;
    this->max_sp = 0;
    this->stat_points = 0;
    this->skill_points = 0;
    this->karma = 0;
    this->min_dam = 0;
    this->max_dam = 0;
    this->accuracy = 0;
    this->evade = 0;
    this->armor = 0;
    this->wis = 0;
    this->intl = 0;
    this->agi = 0;
    this->con = 0;
    this->cha = 0;

    this->weight = 0;
    this->max_weight = 0;

    this->x = 0;
    this->y = 0;
    this->direction = static_cast<Direction>(0);
    this->gender = static_cast<Gender>(0);
    this->hair_style = 0;
    this->hair_color = 0;
    this->race = static_cast<Skin>(0);

    this->sitting = static_cast<SitState>(0);
    this->visibility = 0;
}
