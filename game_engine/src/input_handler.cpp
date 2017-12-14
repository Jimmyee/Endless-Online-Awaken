// Endless Online Awaken

#include "input_handler.hpp"

#include "client.hpp"
#include "map.hpp"
#include "map_editor.hpp"

#include <iostream>

bool InputHandler::initialized_ = false;
bool InputHandler::walk = false;
Direction InputHandler::direction = static_cast<Direction>(0);
std::map<unsigned char, bool> InputHandler::keys;

InputHandler::InputHandler()
{
    if(!this->initialized_)
    {
        walk = false;
        direction = static_cast<Direction>(0);

        this->initialized_ = true;
    }
}

void InputHandler::ProcessEvent(ALLEGRO_EVENT event)
{
    if(event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        this->keys[event.keyboard.keycode] = true;

        if(event.keyboard.keycode >= ALLEGRO_KEY_LEFT && event.keyboard.keycode <= ALLEGRO_KEY_DOWN)
        {
            this->CharacterDirection(event.keyboard);
        }
    }
    if(event.type == ALLEGRO_EVENT_KEY_UP)
    {
        this->keys[event.keyboard.keycode] = false;
    }
}

void InputHandler::Process()
{

}

void InputHandler::CharacterDirection(ALLEGRO_KEYBOARD_EVENT event)
{
    Client client;

    if(event.keycode == ALLEGRO_KEY_UP) client.character->Face(Direction::Up);
    if(event.keycode == ALLEGRO_KEY_RIGHT) client.character->Face(Direction::Right);
    if(event.keycode == ALLEGRO_KEY_DOWN) client.character->Face(Direction::Down);
    if(event.keycode == ALLEGRO_KEY_LEFT) client.character->Face(Direction::Left);
}
