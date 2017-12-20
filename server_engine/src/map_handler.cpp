#include "map_handler.hpp"

#include <iostream>

bool MapHandler::initialized_ = false;
std::map<unsigned int, Map> MapHandler::maps;

MapHandler::MapHandler()
{
    if(!this->initialized_)
    {
        this->initialized_ = true;
    }
}

Map *MapHandler::GetMap(unsigned int id)
{
    for(auto &it: this->maps)
    {
        if(it.first == id) return &it.second;
    }

    std::cout << "Loading map" << std::endl;

    Map map(id);
    this->maps[id] = map;

    return &this->maps[id];
}

void MapHandler::Clear()
{
    this->maps.clear();
}
