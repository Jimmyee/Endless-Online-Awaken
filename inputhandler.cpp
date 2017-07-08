#include "inputhandler.hpp"
#include "singleton.hpp"

InputHandler::InputHandler()
{
    this->walk = false;
    this->direction = static_cast<Direction>(0);
}

void InputHandler::Process(sf::Event event)
{

    if(event.key.code == sf::Keyboard::Up
       || event.key.code == sf::Keyboard::Right
       || event.key.code == sf::Keyboard::Down
       || event.key.code == sf::Keyboard::Left)
    {
        this->Movement(event);
    }
    else if(event.key.code == sf::Keyboard::Escape)
    {
        S::GetInstance().call_exit = true;
    }
    else if(event.key.code == sf::Keyboard::F12)
    {
        S::GetInstance().eoclient.RefreshRequest();
    }
}

void InputHandler::Movement(sf::Event event)
{
    if(event.type == sf::Event::KeyPressed)
    {
        this->walk = true;

        if(event.key.code == sf::Keyboard::Up)
        {
            this->direction = Direction::Up;
        }
        else if(event.key.code == sf::Keyboard::Right)
        {
            this->direction = Direction::Right;
        }
        else if(event.key.code == sf::Keyboard::Down)
        {
            this->direction = Direction::Down;
        }
        else if(event.key.code == sf::Keyboard::Left)
        {
            this->direction = Direction::Left;
        }
    }
    else if(event.type == sf::Event::KeyReleased)
    {
        this->walk = false;
    }

    if(walk)
    {
        S &s = S::GetInstance();
        int xoff[4] = { 0, -1, 0, 1 };
        int yoff[4] = { 1, 0, -1, 0 };

        if(s.map.Walkable(s.character.x + xoff[(int)this->direction], s.character.y + yoff[(int)this->direction])
           && !s.character.animation.play)
        {
            if(s.eoclient.Walk(this->direction))
            {
                s.character.x += xoff[(int)this->direction];
                s.character.y += yoff[(int)this->direction];
                s.character.direction = this->direction;

                s.character.animation.Play();
            }
        }
    }
}
