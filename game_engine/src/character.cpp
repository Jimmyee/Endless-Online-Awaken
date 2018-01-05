// Endless Online Awaken

#include "character.hpp"

#include "gfx_loader.hpp"
#include "client.hpp"
#include "input_handler.hpp"
#include "map.hpp"

#include <allegro5/allegro.h>
#include <iostream>

Character::Character()
{
    this->name = "";
    this->map_id = 0;
    this->x = 0;
    this->y = 0;
    this->direction = (Direction)0;
    this->gender = Gender::Female;
    this->anim_state = AnimState::Stand;
}

Character::Character(std::string name, unsigned short map_id, unsigned short x, unsigned short y)
{
    this->name = name;
    this->map_id = map_id;
    this->x = x;
    this->y = y;
    this->direction = (Direction)0;
    this->gender = Gender::Female;
    this->anim_state = AnimState::Stand;
}

void Character::Tick()
{
    InputHandler input_handler;

    this->animation.Tick();

    if(!this->animation.play && this->animation.current_frame != 0)
    {
        if(this->anim_state == AnimState::Walk)
        {
            this->animation.Clear();
            this->anim_state = AnimState::Stand;
        }
        else
        {
            this->animation.Clear();
        }

        /*if(input_handler.rewalk)
        {
            this->Walk(input_handler.direction);
            input_handler.rewalk = false;
        }*/
    }
}

void Character::Render(int rx, int ry)
{
    GFXLoader gfx_loader;

    ALLEGRO_BITMAP *bitmap = NULL;
    int gfx_offset[3] = { 1, 1, 2 };
    bitmap = gfx_loader.GetBitmap(8, gfx_offset[(int)this->anim_state]);

    int w = al_get_bitmap_width(bitmap);
    int h = al_get_bitmap_height(bitmap);

    int frames_per_anim[3] = { 1, 1, 4 };
    int frames_total = frames_per_anim[(int)this->anim_state] * 4;

    int frame_w = w / frames_total;
    int frame_h = h;

    int screen_x = this->x * 64 - this->x * 32 - this->y * 32 + rx;
    int screen_y = this->y * 16 + this->x * 16 + ry;

    screen_x += 32; // half of the tile width
    screen_y -= frame_h;
    screen_y += 20; // correction for character position on the tile

    int position_off_x[4] = { -1, -1, 1, 1 };
    int position_off_y[4] = { 1, -1, -1, 1 };

    if(this->anim_state == AnimState::Walk)
    {
        screen_x += ((8 * (4 - (int)this->animation.current_frame)) * position_off_x[(int)this->direction]);
        screen_y += ((4 * (4 - (int)this->animation.current_frame)) * position_off_y[(int)this->direction]);
    }

    int dir = (int)this->direction;
    int gender_offset = (int)this->gender * (2 * (frame_w * frames_per_anim[(int)this->anim_state]));

    int dir_offset[4] =
    { (frame_w * frames_per_anim[(int)this->anim_state]) + gender_offset, gender_offset, gender_offset,
    (frame_w * frames_per_anim[(int)this->anim_state]) + gender_offset };

    int frame_offset = frame_w * this->animation.current_frame;

    int cx = frame_w / 2;
    int cy = 0;
    float scale_offset[4] = { -1, -1, 1, 1 };

    al_draw_tinted_scaled_rotated_bitmap_region(bitmap, dir_offset[dir] + frame_offset, 0, frame_w, frame_h,
                                                al_map_rgb(255, 255, 255),
                                                cx, cy, screen_x, screen_y, scale_offset[dir], 1, 0, 0);
}

void Character::Talk(unsigned char channel, std::string message, std::string char_name)
{

}

void Character::Face(Direction direction)
{
    if(this->animation.play || direction == this->direction) return;

    this->direction = direction;

    this->anim_state = AnimState::Stand;
    this->animation.Clear();
    this->animation.frames.push_back(Animation::Frame(2));
    this->animation.Play();
}

void Character::Walk(Direction direction)
{
    if(this->animation.play) return;

    if(direction == Direction::Up && this->y == 0) return;
    if(direction == Direction::Right && this->x == Map().width - 1) return;
    if(direction == Direction::Down && this->y == Map().height - 1) return;
    if(direction == Direction::Left && this->x == 0) return;

    this->direction = direction;

    if(this->direction == Direction::Up) this->y--;
    if(this->direction == Direction::Right) this->x++;
    if(this->direction == Direction::Down) this->y++;
    if(this->direction == Direction::Left) this->x--;

    this->anim_state = AnimState::Walk;
    this->animation.Clear();
    this->animation.frames.push_back(Animation::Frame(1));
    this->animation.frames.push_back(Animation::Frame(1));
    this->animation.frames.push_back(Animation::Frame(1));
    this->animation.frames.push_back(Animation::Frame(1));
    this->animation.Play();
}
