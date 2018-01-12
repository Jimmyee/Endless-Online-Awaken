// Endless Online Awaken

#include "npc.hpp"

#include "gfx_loader.hpp"
#include "util.hpp"

#include <allegro5/allegro.h>
#include <iostream>

NPC::NPC()
{
    this->id = 0;
    this->index = 0;
    this->name = "";
    this->map_id = 0;
    this->x = 0;
    this->y = 0;
    this->direction = (Direction)0;
    this->speed = 1;

    this->anim_state = AnimState::Stand;

    this->screen_x = 0;
    this->screen_y = 0;

    this->frame_counter = 0;
}

NPC::NPC(unsigned int id, unsigned int index, unsigned int map_id, unsigned short x, unsigned short y)
{
    this->id = id;
    this->index = index;
    this->name = "";
    this->map_id = map_id;
    this->x = x;
    this->y = y;
    this->direction = (Direction)0;
    this->speed = 1;

    this->anim_state = AnimState::Stand;

    this->screen_x = 0;
    this->screen_y = 0;

    this->frame_counter = 0;
}

void NPC::Tick()
{
    if(this->frame_counter >= 20 - this->speed)
    {
        this->frame_counter = 0;

        this->animation.Tick();
    }

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
    }

    this->frame_counter++;
}

void NPC::Render(int rx, int ry)
{
    GFXLoader gfx_loader;

    ALLEGRO_BITMAP *bitmap = NULL;

    unsigned int g_id = (this->id - 1) * 40 + 1;

    if(this->anim_state == AnimState::Stand)
    {
        if(this->direction == Direction::Up || this->direction == Direction::Left) g_id += 2;
    }
    if(this->anim_state == AnimState::Walk)
    {
        g_id += 4;

        if(this->direction == Direction::Up || this->direction == Direction::Left) g_id += 4;

        g_id += (int)this->animation.current_frame;
    }

    bitmap = gfx_loader.GetBitmap(21, g_id, true);

    if(bitmap == 0) return;

    int frame_w = al_get_bitmap_width(bitmap);
    int frame_h = al_get_bitmap_height(bitmap);

    this->screen_x = this->x * 64 - this->x * 32 - this->y * 32 + rx;
    this->screen_y = this->y * 16 + this->x * 16 + ry;

    screen_x += 32;
    screen_y += 16;

    if(frame_h > 32)
    {
        screen_y -= frame_h;
        screen_y += 16;
        if(frame_w < 64) screen_y -= 8;
    }
    if(frame_h < 32)
    {
        screen_y -= frame_h / 2;
        screen_y -= 8;
    }

    if(this->anim_state == AnimState::Walk)
    {
        int walk_off_x[4] = { -1, -1, 1, 1 };
        int walk_off_y[4] = { 1, -1, -1, 1 };

        screen_x += ((8 * (4 - (int)this->animation.current_frame)) * walk_off_x[(int)this->direction]);
        screen_y += ((4 * (4 - (int)this->animation.current_frame)) * walk_off_y[(int)this->direction]);
    }

    int dir = (int)this->direction;

    int cx = frame_w / 2;
    int cy = 0;
    float scale_offset[4] = { -1, -1, 1, 1 };

    al_draw_tinted_scaled_rotated_bitmap_region(bitmap, 0, 0, frame_w, frame_h,
                                                al_map_rgb(255, 255, 255),
                                                cx, cy, screen_x, screen_y, scale_offset[dir], 1, 0, 0);
}

std::vector<int> NPC::GetScreenPos()
{
    std::vector<int> ret;

    ret.push_back(this->screen_x);
    ret.push_back(this->screen_y);

    return ret;
}

void NPC::Talk(std::string message)
{

}

void NPC::Face(Direction direction)
{
    if(this->animation.play || direction == this->direction) return;

    this->direction = direction;

    this->anim_state = AnimState::Stand;
    this->animation.Clear();
    this->animation.frames.push_back(Animation::Frame(2));
    this->animation.Play();
}

void NPC::Walk(Direction direction)
{
    if(this->animation.play) return;

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
