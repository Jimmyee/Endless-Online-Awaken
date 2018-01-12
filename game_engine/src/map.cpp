// Endless Online Awaken

#include "map.hpp"

#include "file_handler.hpp"
#include "gui.hpp"
#include "map_cursor.hpp"
#include "font_handler.hpp"
#include "client.hpp"

#include <fstream>
#include <iostream>

bool Map::initialized_ = false;
bool Map::exists;
unsigned int Map::id;
std::string Map::name;
unsigned int Map::revision;
unsigned short Map::width;
unsigned short Map::height;
unsigned int Map::fill_tile;
std::map<Map::Layer::Type, Map::Layer> Map::layers;
std::map<unsigned short, std::map<unsigned short, std::shared_ptr<Map::Attribute>>> Map::atts;
std::vector<std::shared_ptr<Character>> Map::characters;
std::vector<std::shared_ptr<NPC>> Map::npcs;

Map::Layer::Layer()
{
    this->type = Type::Ground;
}

Map::Map()
{
    if(!this->initialized_)
    {
        this->exists = false;
        this->id = 0;
        this->name = "";
        this->revision = 0;
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
    this->revision = fh.GetInt();
    this->width = fh.GetShort();
    this->height = fh.GetShort();
    this->fill_tile = fh.GetInt();

    unsigned char read_tile = fh.GetChar();

    while(read_tile == 1)
    {
        std::shared_ptr<Tile> tile = std::shared_ptr<Tile>(new Tile());

        tile->graphic_id = fh.GetInt();
        tile->x = fh.GetShort();
        tile->y = fh.GetShort();
        Layer::Type layer_type = (Layer::Type)fh.GetChar();

        this->layers[(Layer::Type)layer_type].tiles[tile->x][tile->y] = tile;

        read_tile = fh.GetChar();
    }

    read_tile = fh.GetChar();

    while(read_tile == 1)
    {
        std::shared_ptr<Attribute> att = std::shared_ptr<Attribute>(new Attribute());

        att->type = (Attribute::Type)fh.GetChar();
        att->x = fh.GetShort();
        att->y = fh.GetShort();

        this->atts[att->x][att->y] = att;

        read_tile = fh.GetChar();
    }

    std::cout << "Map loaded: '" << this->name << "' " << this->width << "x" << this->height << std::endl;

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
    fh.AddInt(++this->revision);
    fh.AddShort(this->width);
    fh.AddShort(this->height);
    fh.AddInt(this->fill_tile);

    for(auto &l : this->layers)
    {
        for(auto &t : l.second.tiles)
        {
            for(auto &tt : t.second)
            {
                std::shared_ptr<Tile> tile = tt.second;

                if(tile == 0) continue;

                fh.AddChar(1);
                fh.AddInt(tile->graphic_id);
                fh.AddShort(tile->x);
                fh.AddShort(tile->y);
                fh.AddChar((unsigned int)l.first);
            }
        }
    }

    fh.AddChar(0);

    for(auto &a : this->atts)
    {
        for(auto &aa : a.second)
        {
            std::shared_ptr<Attribute> att = aa.second;

            if(att == 0) continue;

            fh.AddChar(1);
            fh.AddChar((unsigned int)att->type);
            fh.AddShort(att->x);
            fh.AddShort(att->y);
        }
    }

    fh.AddChar(0);

    fh.Save(filename);
    this->exists = fh.Exists();
}

void Map::Tick()
{
    for(auto &it : this->characters)
    {
        it->Tick();
    }

    for(auto &it : this->npcs)
    {
        it->Tick();
    }
}

void Map::Render(int rx, int ry)
{
    Character *character = Client().character;
    unsigned short start_x = 0;
    unsigned short start_y = 0;
    unsigned short end_x = this->width > 0? this->width - 1 : 1;
    unsigned short end_y = this->height > 0? this->height - 1 : 1;

    if(character != 0)
    {
        start_x = character->x > 16? character->x - 16 : 0;
        start_y = character->y > 16? character->y - 16 : 0;
        end_x = character->x + 16 > this->width? this->width - 1 : character->x + 16;
        end_y = character->y + 16 > this->height? this->height - 1 : character->y + 16;
    }

    if(this->fill_tile != 0)
    {
        ALLEGRO_BITMAP *tilegfx = GFXLoader().GetBitmap(3, this->fill_tile);

        if(tilegfx != 0)
        {
            al_hold_bitmap_drawing(true);

            for(unsigned short x = start_x; x <= end_x; ++x)
            {
                for(unsigned short y = start_y; y <= end_y; ++y)
                {
                    int screen_x = x * 64 - x * 32 - y * 32 + rx;
                    int screen_y = y * 16 + x * 16 + ry;

                    al_draw_bitmap(tilegfx, screen_x, screen_y, 0);
                }
            }

            al_hold_bitmap_drawing(false);
        }
    }

    int gid_offset[2] = { 3, 4 };
    bool cursor_drawn = false;

    for(int i = 0; i < (int)Layer::Type::Object; ++i) // render stagged tiles for ground layer only
    {
        std::map<unsigned int, ALLEGRO_BITMAP *> bitmaps;
        std::map<unsigned int, std::vector<std::shared_ptr<Tile>>> tiles_stagged;

        for(unsigned short x = start_x; x <= end_x; ++x)
        {
            for(unsigned short y = start_y; y <= end_y; ++y)
            {
                std::shared_ptr<Tile> tile = this->layers[(Layer::Type)i].tiles[x][y];

                if(tile == 0) continue;

                ALLEGRO_BITMAP *tilegfx = NULL;

                if(bitmaps.find(tile->graphic_id) == bitmaps.end())
                {
                    tilegfx = GFXLoader().GetBitmap(gid_offset[i], tile->graphic_id);
                    bitmaps[tile->graphic_id] = tilegfx;
                }

                if(tile->x >= start_x && tile->x <= end_x && tile->y >= start_y && tile->y <= end_y)
                    tiles_stagged[tile->graphic_id].push_back(tile);
            }
        }

        for(auto &ts : tiles_stagged)
        {
            al_hold_bitmap_drawing(true);
            for(auto &tile : ts.second)
            {
                if(bitmaps[ts.first] == NULL) continue;

                int screen_x = tile->x * 64 - tile->x * 32 - tile->y * 32 + rx;
                int screen_y = tile->y * 16 + tile->x * 16 + ry;

                int w = al_get_bitmap_width(bitmaps[ts.first]);
                int h = al_get_bitmap_height(bitmaps[ts.first]);

                if(i == (int)Layer::Type::Object)
                {
                    if(w > 64)
                    {
                        screen_x -= w / 2;
                        screen_x += 32;
                    }
                    if(h > 32)
                    {
                        screen_y -= h;
                        screen_y += 32;
                    }
                    if(w < 64)
                    {
                        screen_x -= w / 2;
                        screen_x += 32;
                        if(h > 32) screen_y -= 8;
                    }
                    if(h < 32)
                    {
                        screen_y -= h / 2;
                        screen_y += 16;
                    }
                }

                al_draw_bitmap(bitmaps[ts.first], screen_x, screen_y, 0);
            }
            al_hold_bitmap_drawing(false);
        }

        if(i == (int)Layer::Type::Ground)
        {
            MapCursor().Render(rx, ry);
            cursor_drawn = true;
        }
    }

    if(!cursor_drawn)
    {
        MapCursor().Render(rx, ry);
    }

    // object layer + characters
    for(unsigned short x = start_x; x <= end_x; ++x)
    {
        for(unsigned short y = start_y; y <= end_y; ++y)
        {
            std::shared_ptr<Tile> tile = this->layers[Layer::Type::Object].tiles[x][y];

            if(tile != 0)
            {
                ALLEGRO_BITMAP *tilegfx = GFXLoader().GetBitmap(gid_offset[(int)Layer::Type::Object], tile->graphic_id);

                if(tilegfx != 0)
                {
                    int screen_x = tile->x * 64 - tile->x * 32 - tile->y * 32 + rx;
                    int screen_y = tile->y * 16 + tile->x * 16 + ry;

                    int w = al_get_bitmap_width(tilegfx);
                    int h = al_get_bitmap_height(tilegfx);

                    if(w > 64)
                    {
                        screen_x -= w / 2;
                        screen_x += 32;
                    }
                    if(h > 32)
                    {
                        screen_y -= h;
                        screen_y += 32;
                    }
                    if(w < 64)
                    {
                        screen_x -= w / 2;
                        screen_x += 32;
                        if(h > 32) screen_y -= 8;
                    }
                    if(h < 32)
                    {
                        screen_y -= h / 2;
                        screen_y += 16;
                    }

                    al_draw_bitmap(tilegfx, screen_x, screen_y, 0);
                }
            }

            for(auto &charr : this->characters)
            {
                if(charr->x == x && charr->y == y) charr->RenderNew(rx, ry);
            }

            for(auto &npc : this->npcs)
            {
                if(npc->x == x && npc->y == y) npc->Render(rx, ry);
            }
        }
    }

    std::string nickname = MapCursor().nickname;

    if(nickname != "")
    {
        ALLEGRO_FONT *font = FontHandler().font;
        float alpha = 0.5f;
        ALLEGRO_COLOR color = al_map_rgba_f(1.0, 1.0, 1.0, alpha);

        Character *charr = this->GetCharacter(nickname);

        if(charr == 0) return;

        int text_w = al_get_text_width(font, nickname.c_str());

        std::vector<int> char_pos = charr->GetScreenPos();
        al_draw_text(font, color, char_pos[0] - text_w / 2, char_pos[1] - 6, 0, nickname.c_str());

    }
}

void Map::Reset()
{
    this->id = 0;
    this->name.clear();
    this->revision = 0;
    this->width = 1;
    this->height = 1;
    this->fill_tile = 0;

    this->layers.clear();
    this->atts.clear();
    this->characters.clear();
    this->npcs.clear();
}

std::vector<Character *> Map::GetCharactersAt(unsigned short x, unsigned short y)
{
    std::vector<Character *> ret;

    for(auto &it: characters)
    {
        if(it->x == x && it->y == y)
        {
            ret.push_back(it.get());
        }
    }

    return ret;
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

std::vector<NPC *> Map::GetNPCsAt(unsigned short x, unsigned short y)
{
    std::vector<NPC *> ret;

    for(auto &it: npcs)
    {
        if(it->x == x && it->y == y)
        {
            ret.push_back(it.get());
        }
    }

    return ret;
}

NPC *Map::GetNPC(unsigned int index)
{
    for(auto &it: npcs)
    {
        if(it->index == index)
            return it.get();
    }

    return 0;
}

void Map::DeleteNPC(unsigned int index)
{
    for(std::size_t i = 0; i < this->npcs.size(); ++i)
    {
        if(this->npcs[i]->index == index)
        {
            this->npcs.erase(this->npcs.begin() + i);
            return;
        }
    }
}

bool Map::Walkable(unsigned short x, unsigned short y, bool entity)
{
    if(this->atts[x][y] != 0)
    {
        if(this->atts[x][y]->type == Attribute::Type::Wall) return false;
    }

    if(!entity) return true;

    for(auto &it: this->characters)
    {
        if(it->x == x && it->y == y) return false;
    }

    for(auto &it: this->npcs)
    {
        if(it->x == x && it->y == y) return false;
    }

    return true;
}
