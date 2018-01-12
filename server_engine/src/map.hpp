// Endless Online Awaken

#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include <SFML/System.hpp>
#include "character.hpp"
#include "npc.hpp"

#include <string>
#include <array>
#include <vector>
#include <map>

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
    bool exists;

public:
    unsigned int id;
    std::string name;
    unsigned int revision;
    unsigned short width;
    unsigned short height;
    unsigned int fill_tile;

    std::map<Layer::Type, Layer> layers;
    std::map<unsigned short, std::map<unsigned short, std::shared_ptr<Attribute>>> atts;

    std::vector<std::shared_ptr<Character>> characters;
    std::vector<std::shared_ptr<NPC>> npcs;

    sf::Clock npc_clock;

    Map();
    Map(unsigned int id);
    bool Load(unsigned int id);
    void Save();
    void Tick();
    bool Exists() { return this->exists; }
    void Reset();

    std::vector<Character *> GetCharactersAt(unsigned short x, unsigned short y);
    Character *GetCharacter(std::string name);
    void DeleteCharacter(std::string name);

    std::vector<NPC *> GetNPCsAt(unsigned short x, unsigned short y);
    NPC *GetNPC(unsigned int index);
    void AddNPC(std::shared_ptr<NPC> npc);
    void DeleteNPC(unsigned int index);

    bool Walkable(unsigned short x, unsigned short y, bool entity = true);

    unsigned int GenerateNPCIndex();
};

#endif // MAP_HPP_INCLUDED
