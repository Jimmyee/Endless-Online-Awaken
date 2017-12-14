// Endless Online Awaken

#ifndef MAP_EDITOR_HPP_INCLUDED
#define MAP_EDITOR_HPP_INCLUDED

#include <allegro5/allegro5.h>
#include <memory>

// (monostate)
class MapEditor
{
private:
    static bool initialized_;

public:
    static int render_xoff;
    static int render_yoff;

    MapEditor();
    void MakeGUI();
    void ProcessInput();
};

#endif // MAP_EDITOR_HPP_INCLUDED
