#ifndef MAP_HANDLER_HPP_INCLUDED
#define MAP_HANDLER_HPP_INCLUDED

#include "map.hpp"

#include <map>

// (monostate)
class MapHandler
{
public:
    static std::map<unsigned int, Map> maps;

    Map *GetMap(unsigned int id);
    void Clear();
};

#endif // MAP_HANDLER_HPP_INCLUDED
