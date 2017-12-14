// Endless Online Awaken

#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include "gfx_loader.hpp"
#include "character.hpp"

#include <string>
#include <array>
#include <vector>

// (monostate)
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
    static bool initialized_;
    static bool exists;

public:
    static unsigned int id;
    static std::string name;
    static unsigned short width;
    static unsigned short height;
    static unsigned int fill_tile;

    static std::array<std::vector<Tile>, 1> tiles;

    static std::vector<Character> characters;

    Map();
    Map(unsigned int id);
    bool Load(unsigned int id);
    void Save();
    bool Exists() { return this->exists; }
    void Render(int rx, int ry);
    Tile GetTile(unsigned char layer, unsigned short x, unsigned short y);
    void Reset();

    Character *GetCharacter(unsigned short x, unsigned short y);
    Character *GetCharacter(unsigned int id);
    Character *GetCharacter(std::string name);
    void DeleteCharacter(std::string name);
    //void UpdateCharacter(std::string name, Character character);
};

#endif // MAP_HPP_INCLUDED
