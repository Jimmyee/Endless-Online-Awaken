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

    MapCursor();
    void Render(int rx, int ry);
};

#endif // MAP_CURSOR_HPP_INCLUDED
