// Endless Online Awaken

#include "character.hpp"

#include "gfx_loader.hpp"
#include "client.hpp"

#include <allegro5/allegro.h>
#include <iostream>

Character::Character()
{
    this->name = "";
    this->id = 0;
    this->map_id = 0;
    this->x = 0;
    this->y = 0;
    this->direction = (Direction)0;
    this->gender = Gender::Female;
}

Character::Character(std::string name, unsigned int id, unsigned short map_id, unsigned short x, unsigned short y)
{
    this->name = name;
    this->id = id;
    this->map_id = map_id;
    this->x = x;
    this->y = y;
    this->direction = (Direction)0;
    this->gender = Gender::Female;
}

void Character::Render(int rx, int ry)
{
    GFXLoader gfx_loader;

    ALLEGRO_BITMAP *bitmap = NULL;
    bitmap = gfx_loader.GetBitmap(8, 1);

    int w = al_get_bitmap_width(bitmap);
    int h = al_get_bitmap_height(bitmap);

    int frame_w = w / 4;
    int frame_h = h;

    int screen_x = this->x * 64 - this->x * 32 - this->y * 32 + (frame_w / 2 + 16) + rx;
    int screen_y = this->y * 16 + this->x * 16 - frame_h + 20 + ry;

    int dir = (int)this->direction;
    int gender_offset = (int)this->gender * frame_w;
    int dir_offset[4] = { frame_w + gender_offset, 0, 0, frame_w + gender_offset };
    int cx = frame_w / 2;
    int cy = 0;
    float scale_offset[4] = { -1, -1, 1, 1 };

    al_draw_tinted_scaled_rotated_bitmap_region(bitmap, dir_offset[dir], 0, w / 4, h, al_map_rgb(255, 255, 255),
                                                cx, cy, screen_x, screen_y, scale_offset[dir], 1, 0, 0);
}

void Character::Face(Direction direction)
{
    this->direction = direction;

    std::cout << "FACE" << std::endl;

    sf::Packet packet;
    packet << (unsigned short)PacketID::Character;
    packet << (unsigned char)5;
    packet << (unsigned char)this->direction;
    Client().Send(packet);
}

void Character::Talk(unsigned char channel, std::string message, std::string char_name)
{
    sf::Packet packet;
    packet << (unsigned short)PacketID::Character;
    packet << (unsigned char)6;
    packet << channel;
    packet << message;
    Client().Send(packet);
}
