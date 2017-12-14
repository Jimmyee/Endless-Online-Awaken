// Endless Online Awaken

#include "map.hpp"
#include "file_handler.hpp"

#include <fstream>
#include <iostream>

Map::Map()
{
    this->exists = false;
    this->id = 0;
    this->width = 1;
    this->height = 1;
    this->fill_tile = 0;
}

Map::Map(unsigned int id)
{
    this->Load(id);
}

bool Map::Load(unsigned int id)
{
    this->Reset();
    this->exists = false;

    this->id = id;

    if(this->id == 0)
    {
        throw std::runtime_error("Map: can't load map, ID #0 is reserved.");
    }

    std::string filename = "data/maps/" + std::to_string(this->id) + ".map";
    FileHandler fh(filename);

    this->exists = fh.Exists();

    if(!this->exists) return false;

    this->name = fh.GetString();
    this->width = fh.GetShort();
    this->height = fh.GetShort();
    this->fill_tile = fh.GetInt();

    std::size_t tiles = fh.GetInt();
    for(std::size_t i = 0; i < tiles; ++i)
    {
        Tile tile;
        tile.graphic_id = fh.GetInt();
        tile.x = fh.GetShort();
        tile.y = fh.GetShort();
        tile.type = (Tile::Type)fh.GetChar();

        this->tiles[0].push_back(tile);
    }

    std::cout << "Map loaded: '" << this->name << "' " << this->width << "x" << this->height << std::endl;
    std::cout << "Fill tile: " << this->fill_tile << std::endl;

    return true;
}

void Map::Save()
{
    if(this->id == 0)
    {
        throw std::runtime_error("Map: can't save map, ID #0 is reserved.");
    }

    std::ofstream file;
    std::string filename = "data/maps/" + std::to_string(this->id) + ".map";
    FileHandler fh;

    fh.AddString(this->name);
    fh.AddShort(this->width);
    fh.AddShort(this->height);
    fh.AddInt(this->fill_tile);

    std::size_t tiles = this->tiles[0].size();
    fh.AddInt(tiles);

    for(std::size_t i = 0; i < this->tiles[0].size(); ++i)
    {
        fh.AddInt(this->tiles[0][i].graphic_id);
        fh.AddShort(this->tiles[0][i].x);
        fh.AddShort(this->tiles[0][i].y);
        fh.AddChar((unsigned char)this->tiles[0][i].type);
    }

    fh.Save(filename);
    this->exists = fh.Exists();
}

void Map::Reset()
{
    this->id = 0;
    this->name.clear();
    this->width = 1;
    this->height = 1;
    this->fill_tile = 0;
    this->tiles[0].clear();
    this->characters.clear();
}

Character *Map::GetCharacter(unsigned short x, unsigned short y)
{
    for(auto &it: characters)
    {
        if(it.x == x && it.y == y)
            return &it;
    }

    return 0;
}

Character *Map::GetCharacter(unsigned int id)
{
    for(auto &it: characters)
    {
        if(it.id == id)
            return &it;
    }

    return 0;
}

Character *Map::GetCharacter(std::string name)
{
    for(auto &it: characters)
    {
        if(it.name == name)
            return &it;
    }

    return 0;
}

void Map::DeleteCharacter(std::string name)
{
    for(std::size_t i = 0; i < this->characters.size(); ++i)
    {
        if(this->characters[i].name == name)
        {
            this->characters.erase(this->characters.begin() + i);
            return;
        }
    }
}
