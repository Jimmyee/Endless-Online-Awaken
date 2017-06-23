// Endless Online Awaken v0.0.1

#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include "eodata.hpp"
#include "character.hpp"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

using std::shared_ptr;

// class responsible for rendering the map and holding data about map's objects
class Map
{
public:
    sf::RenderWindow *window;
    shared_ptr<EMF> emf;
    std::vector<shared_ptr<Character>> characters;

    Map(int id);
    Map(shared_ptr<EMF> emf);
    void Load(int id);

    shared_ptr<Character> GetCharacter(short gameworld_id);
    shared_ptr<Character> GetCharacter(std::string name);
    void RemoveCharacter(short gameworld_id);
};

#endif // MAP_HPP_INCLUDED
