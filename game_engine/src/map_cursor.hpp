#ifndef MAP_CURSOR_HPP_INCLUDED
#define MAP_CURSOR_HPP_INCLUDED

#include <string>

// (monostate)
class MapCursor
{
private:
    static bool initialized_;

public:
    static std::string nickname;
    static unsigned short x;
    static unsigned short y;
    static int rx;
    static int ry;

    MapCursor();
    void Render(int rx, int ry);
    bool TestMousePos(int mx, int my, unsigned short x, unsigned short y);
    bool MouseOnMap();
};

#endif // MAP_CURSOR_HPP_INCLUDED
