// Endless Online Awaken

#ifndef MAP_EDITOR_HPP_INCLUDED
#define MAP_EDITOR_HPP_INCLUDED

#include "map.hpp"

#include <allegro5/allegro5.h>
#include <memory>

// (monostate)
class MapEditor
{
private:
    static bool initialized_;
    static unsigned int graphic_id;
    static Map::Layer::Type type;

public:
    static int render_xoff;
    static int render_yoff;

    MapEditor();
    void MakeGUI();
    void ProcessInput();
    void Reset();
};

#endif // MAP_EDITOR_HPP_INCLUDED
