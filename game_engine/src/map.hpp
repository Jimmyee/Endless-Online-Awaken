// Endless Online Awaken

#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include "gfx_loader.hpp"
#include "character.hpp"
#include "npc.hpp"

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
        unsigned int graphic_id;
        unsigned short x;
        unsigned short y;

        Tile() : graphic_id(0), x(0), y(0) { }
    };

    class Layer
    {
    public:
        enum class Type
        {
            Ground,
            Object
        };

    public:
        Type type;
        std::map<unsigned short, std::map<unsigned short, std::shared_ptr<Tile>>> tiles;

        Layer();
    };

    class Attribute
    {
    public:
        enum class Type
        {
            Wall
        };

    public:
        Type type;
        unsigned short x;
        unsigned short y;

        Attribute() : type(Type::Wall), x(0), y(0) { }
    };

private:
    static bool initialized_;
    static bool exists;

public:
    static unsigned int id;
    static std::string name;
    static unsigned int revision;
    static unsigned short width;
    static unsigned short height;
    static unsigned int fill_tile;

    static std::map<Layer::Type, Layer> layers;
    static std::map<unsigned short, std::map<unsigned short, std::shared_ptr<Attribute>>> atts;

    static std::vector<std::shared_ptr<Character>> characters;
    static std::vector<std::shared_ptr<NPC>> npcs;

    Map();
    Map(unsigned int id);
    bool Load(unsigned int id);
    void Save();
    bool Exists() { return this->exists; }
    void Tick();
    void Render(int rx, int ry);
    void Reset();

    std::vector<Character *> GetCharactersAt(unsigned short x, unsigned short y);
    Character *GetCharacter(std::string name);
    void DeleteCharacter(std::string name);

    std::vector<NPC *> GetNPCsAt(unsigned short x, unsigned short y);
    NPC *GetNPC(unsigned int index);
    void DeleteNPC(unsigned int index);

    bool Walkable(unsigned short x, unsigned short y, bool entity = true);
};

#endif // MAP_HPP_INCLUDED
