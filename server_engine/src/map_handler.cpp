#include "map_handler.hpp"

std::map<unsigned int, Map> MapHandler::maps;

Map *MapHandler::GetMap(unsigned int id)
{
    for(auto &it: this->maps)
    {
        if(it.first == id) return &it.second;
    }

    Map map(id);
    this->maps[id] = map;

    return &this->maps[id];
}

void MapHandler::Clear()
{
    this->maps.clear();
}
