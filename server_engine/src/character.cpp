// Endless Online Awaken

#include "character.hpp"

#include <allegro5/allegro.h>

Character::Character()
{
    this->username = "";
    this->name = "";
    this->id = 0;
    this->map_id = 0;
    this->x = 0;
    this->y = 0;
    this->direction = static_cast<Direction>(0);
    this->gender = Gender::Female;
}

Character::Character(std::string username, std::string name, unsigned int id, unsigned short map_id, unsigned short x, unsigned short y)
{
    this->username = username;
    this->name = name;
    this->id = id;
    this->map_id = map_id;
    this->x = x;
    this->y = y;
    this->direction = static_cast<Direction>(0);
    this->gender = Gender::Female;
}

void Character::Render(int rx, int ry)
{
    //GFXLoader gfx_loader;

    int screen_x = this->x * 64 - this->x * 32 - this->y * 32 + rx;
    int screen_y = this->y * 16 + this->x * 16 + ry;

    //ALLEGRO_BITMAP *bitmap = NULL;
    //bitmap = gfx_loader.GetBitmap(8, 1);
}
