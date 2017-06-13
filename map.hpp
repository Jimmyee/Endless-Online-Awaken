#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include "eodata.hpp"

#include <SFML/Graphics.hpp>
#include <memory>

// class responsible for rendering the map
class Map
{
public:
    sf::RenderWindow *window;

    Map(int id);
    void Load(int id);
};

#endif // MAP_HPP_INCLUDED
