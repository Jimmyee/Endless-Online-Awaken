#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <memory>

#include "eodata.hpp"

class Map
{
public:
    std::shared_ptr<EMF> emf;
    sf::RenderWindow *window;

    Map();
    void Load(int id);
};

#endif // MAP_HPP_INCLUDED
