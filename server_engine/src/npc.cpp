// Endless Online Awaken

#include "npc.hpp"

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
