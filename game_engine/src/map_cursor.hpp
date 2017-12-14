#ifndef MAP_CURSOR_HPP_INCLUDED
#define MAP_CURSOR_HPP_INCLUDED

// (monostate)
class MapCursor
{
private:
    static bool initialized_;

public:
    MapCursor();
    void Render(int rx, int ry);
};

#endif // MAP_CURSOR_HPP_INCLUDED
