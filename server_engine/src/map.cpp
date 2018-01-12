// Endless Online Awaken

#include "map.hpp"

#include "file_handler.hpp"
#include "client.hpp"
#include "server.hpp"
#include "util.hpp"

#include <fstream>
#include <iostream>

Map::Layer::Layer()
{
    this->type = Type::Ground;
}

Map::Map()
{
    this->exists = false;
    this->id = 0;
    this->name = "";
    this->revision = 0;
    this->width = 1;
    this->height = 1;
    this->fill_tile = 0;

    this->npc_clock.restart();
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

        /*if(it->anim_state == NPC::AnimState::Stand)
        {
            Direction direction = (Direction)RandGen().RandInt(0, 3);

            if(direction == Direction::Up && it->y == 0) continue;
            if(direction == Direction::Right && it->x == this->width - 1) continue;
            if(direction == Direction::Down && it->y == this->height - 1) continue;
            if(direction == Direction::Left && it->x == 0) continue;

            unsigned int walk_x = it->x;
            unsigned int walk_y = it->y;

            if(direction == Direction::Up) walk_y--;
            if(direction == Direction::Right) walk_x++;
            if(direction == Direction::Down) walk_y++;
            if(direction == Direction::Left) walk_x--;

            if(!this->Walkable(walk_x, walk_y)) continue;

            it->Walk(direction);

            std::vector<std::shared_ptr<Character>> chars_in_range;
            for(auto &it : this->characters)
            {
                int len = path_length(it->x, it->y, it->x, it->y);

                if(len < 13)
                {
                    chars_in_range.push_back(it);
                }
            }

            for(auto &charr : chars_in_range)
            {
                Client *client = Server().GetClientByChar(charr->name);

                sf::Packet packet;

                packet << (unsigned int)PacketID::NPC;
                packet << (unsigned int)5; // walk
                packet << it->index;
                packet << (unsigned int)it->direction;
                packet << it->x;
                packet << it->y;

                client->Send(packet);
            }
        }*/
    }

    if(this->npc_clock.getElapsedTime().asMilliseconds() >= 460) this->npc_clock.restart();
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

    this->npc_clock.restart();
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

void Map::AddNPC(std::shared_ptr<NPC> npc)
{
    npc->index = this->GenerateNPCIndex();

    this->npcs.push_back(npc);

    std::vector<std::shared_ptr<Character>> chars_in_range;
    for(auto &it : this->characters)
    {
        int len = path_length(npc->x, npc->y, it->x, it->y);

        if(len < 13)
        {
            chars_in_range.push_back(it);
        }
    }

    for(auto &it : chars_in_range)
    {
        Client *client = Server().GetClientByChar(it->name);

        it->npcs_in_range.push_back(npc);

        sf::Packet packet;

        packet << (unsigned char)PacketID::Map;
        packet << (unsigned char)1; // appear
        packet << (unsigned char)2; // npc
        packet << npc->id;
        packet << npc->index;
        packet << npc->map_id;
        packet << npc->x;
        packet << npc->y;
        packet << (unsigned char)npc->direction;
        packet << npc->speed;

        client->Send(packet);
    }
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

unsigned int Map::GenerateNPCIndex()
{
    unsigned int counter = 1;

    for(auto &it: this->npcs)
    {
        if(it->index != counter)
        {
            return counter;
        }

        counter++;
    }

    return counter;
}
