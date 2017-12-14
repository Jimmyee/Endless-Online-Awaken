// Endless Online Awaken

#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include "character.hpp"

#include <string>
#include <array>
#include <vector>

class Map
{
public:
    class Tile
    {
    public:
        enum class Type : unsigned char
        {
            Ground
        };

    public:
        unsigned int graphic_id;
        unsigned short x;
        unsigned short y;
        Type type;

        Tile() : graphic_id(0), x(0), y(0), type(Type::Ground) { }
    };

private:
    bool initialized_;
    bool exists;

public:
    unsigned int id;
    std::string name;
    unsigned short width;
    unsigned short height;
    unsigned int fill_tile;

    std::array<std::vector<Tile>, 1> tiles;

    std::vector<Character> characters;

    Map();
    Map(unsigned int id);
    bool Load(unsigned int id);
    void Save();
    bool Exists() { return this->exists; }
    Tile GetTile(unsigned char layer, unsigned short x, unsigned short y);
    void Reset();

    Character *GetCharacter(unsigned short x, unsigned short y);
    Character *GetCharacter(unsigned int id);
    Character *GetCharacter(std::string name);
    void DeleteCharacter(std::string name);
};

#endif // MAP_HPP_INCLUDED
