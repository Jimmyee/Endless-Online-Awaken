#ifndef MAP_HANDLER_HPP_INCLUDED
#define MAP_HANDLER_HPP_INCLUDED

#include "map.hpp"

#include <map>

// (monostate)
class MapHandler
{
private:
    static bool initialized_;

public:
    static std::map<unsigned int, Map> maps;

    MapHandler();
    Map *GetMap(unsigned int id);
    void Clear();
    void Reload();
};

#endif // MAP_HANDLER_HPP_INCLUDED
