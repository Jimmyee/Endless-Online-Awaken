// Endless Online Awaken

#include "map.hpp"

#include "file_handler.hpp"
#include "gui.hpp"
#include "map_cursor.hpp"

#include <fstream>
#include <iostream>

bool Map::initialized_ = false;
bool Map::exists;
unsigned int Map::id;
std::string Map::name;
unsigned short Map::width;
unsigned short Map::height;
unsigned int Map::fill_tile;
std::array<std::vector<std::shared_ptr<Map::Tile>>, 1> Map::tiles;
std::vector<std::shared_ptr<Character>> Map::characters;

Map::Map()
{
    if(!this->initialized_)
    {
        this->exists = false;
        this->id = 0;
        this->name = "";
        this->width = 1;
        this->height = 1;
        this->fill_tile = 0;

        this->initialized_ = true;
    }
}

Map::Map(unsigned int id)
{
    if(!this->initialized_)
    {
        this->exists = false;
        this->id = 0;
        this->name = "";
        this->width = 1;
        this->height = 1;
        this->fill_tile = 0;

        this->initialized_ = true;
    }

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
        std::shared_ptr<Tile> tile = std::shared_ptr<Tile>(new Tile());
        tile->graphic_id = fh.GetInt();
        tile->x = fh.GetShort();
        tile->y = fh.GetShort();
        tile->type = (Tile::Type)fh.GetChar();

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
        fh.AddInt(this->tiles[0][i]->graphic_id);
        fh.AddShort(this->tiles[0][i]->x);
        fh.AddShort(this->tiles[0][i]->y);
        fh.AddChar((unsigned char)this->tiles[0][i]->type);
    }

    fh.Save(filename);
    this->exists = fh.Exists();
}

void Map::Render(int rx, int ry)
{
    if(this->fill_tile != 0)
    {
        ALLEGRO_BITMAP *tilegfx = GFXLoader().GetBitmap(3, this->fill_tile);

        al_hold_bitmap_drawing(true);
        for(int x = 0; x < this->width; ++x)
        {
            for(int y = 0; y < this->height; ++y)
            {
                int screen_x = x * 64 - x * 32 - y * 32 + rx;
                int screen_y = y * 16 + x * 16 + ry;

                if(tilegfx != 0) al_draw_bitmap(tilegfx, screen_x, screen_y, 0);
            }
        }
        al_hold_bitmap_drawing(false);
    }

    std::map<unsigned int, ALLEGRO_BITMAP *> bitmaps;
    std::map<unsigned int, std::vector<std::shared_ptr<Tile>>> tiles_stagged;

    for(auto &it : this->tiles[0])
    {
        ALLEGRO_BITMAP *tilegfx = NULL;
        if(bitmaps.find(it->graphic_id) == bitmaps.end())
        {
            tilegfx = GFXLoader().GetBitmap(3, it->graphic_id);
            bitmaps[it->graphic_id] = tilegfx;
        }

        tiles_stagged[it->graphic_id].push_back(it);
    }

    for(auto &ts : tiles_stagged)
    {
        al_hold_bitmap_drawing(true);
        for(auto &it : ts.second)
        {
            int screen_x = it->x * 64 - it->x * 32 - it->y * 32 + rx;
            int screen_y = it->y * 16 + it->x * 16 + ry;

            al_draw_bitmap(bitmaps[ts.first], screen_x, screen_y, 0);
        }
        al_hold_bitmap_drawing(false);
    }

    MapCursor().Render(rx, ry);

    for(auto &it : this->characters)
    {
        it->Render(rx, ry);
    }
}

Map::Tile *Map::GetTile(unsigned char layer, unsigned short x, unsigned short y)
{
    for(auto &it : this->tiles[layer])
    {
        if(it->x == x && it->y == y)
        {
            return it.get();
        }
    }

    return 0;
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
        if(it->x == x && it->y == y)
            return it.get();
    }

    return 0;
}

Character *Map::GetCharacter(unsigned int id)
{
    for(auto &it: characters)
    {
        if(it->id == id)
            return it.get();
    }

    return 0;
}

Character *Map::GetCharacter(std::string name)
{
    for(auto &it: characters)
    {
        if(it->name == name)
            return it.get();
    }

    return 0;
}

void Map::DeleteCharacter(std::string name)
{
    for(std::size_t i = 0; i < this->characters.size(); ++i)
    {
        if(this->characters[i]->name == name)
        {
            this->characters.erase(this->characters.begin() + i);
            return;
        }
    }
}

/*void Map::UpdateCharacter(std::string name, Character character)
{
    for(auto &it : this->characters)
    {
        if(it.name == name)
        {
            std::cout << "dir changed from " << (int)it.direction << " to ";
            it = character;
            std::cout << (int)it.direction << std::endl;
            return;
        }
    }
}*/
