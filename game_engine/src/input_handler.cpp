// Endless Online Awaken

#include "input_handler.hpp"

#include "client.hpp"
#include "map.hpp"
#include "map_editor.hpp"

#include <iostream>

bool InputHandler::initialized_ = false;
std::map<unsigned char, bool> InputHandler::keys;
Direction InputHandler::direction;
bool InputHandler::rewalk;

InputHandler::InputHandler()
{
    if(!this->initialized_)
    {
        this->direction = static_cast<Direction>(0);
        this->rewalk = false;

        this->initialized_ = true;
    }
}

void InputHandler::ProcessEvent(ALLEGRO_EVENT event)
{
    Client client;

    if(event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        this->keys[event.keyboard.keycode] = true;
    }
    if(event.type == ALLEGRO_EVENT_KEY_UP)
    {
        this->keys[event.keyboard.keycode] = false;
    }
}

void InputHandler::Process()
{
    Client client;

    if(client.state != Client::State::Playing) return;

    if(client.character->anim_state == Character::AnimState::Stand || client.character->anim_state == Character::AnimState::Walk)
    {
        this->CharacterMovement();
    }
}

void InputHandler::CharacterMovement()
{
    Client client;

    bool walk = false;
    bool do_rewalk = false;

    for(int i = ALLEGRO_KEY_LEFT; i <= ALLEGRO_KEY_DOWN; ++i)
    {
        if(this->keys[i])
        {
            if(client.character->anim_state == Character::AnimState::Stand)
            {
                walk = true;
            }
            if(client.character->anim_state == Character::AnimState::Walk)
            {
                this->rewalk = true;
                do_rewalk = true;
            }

            break;
        }
    }

    if(this->keys[ALLEGRO_KEY_UP]) this->direction = Direction::Up;
    else if(this->keys[ALLEGRO_KEY_RIGHT]) this->direction = Direction::Right;
    else if(this->keys[ALLEGRO_KEY_DOWN]) this->direction = Direction::Down;
    else if(this->keys[ALLEGRO_KEY_LEFT]) this->direction = Direction::Left;

    if((walk || this->rewalk) && client.character->anim_state == Character::AnimState::Stand && !client.character->animation.play)
    {
        if(client.character->direction != this->direction && !this->rewalk)
        {
            client.character->Face(this->direction);
            client.Face(this->direction);
        }
        else
        {
            client.character->Walk(this->direction);
            client.Walk(this->direction);
        }
    }

    this->rewalk = do_rewalk;
}
