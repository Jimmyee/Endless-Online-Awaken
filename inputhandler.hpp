#ifndef INPUTHANDLER_HPP_INCLUDED
#define INPUTHANDLER_HPP_INCLUDED

#include "const/character.hpp"

#include <SFML/Graphics.hpp>

class InputHandler
{
public:
    bool walk;
    Direction direction;

    InputHandler();
    void Process(sf::Event event);
    void Movement(sf::Event event);
};

#endif // INPUTHANDLER_HPP_INCLUDED
