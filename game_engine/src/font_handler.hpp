#ifndef FONT_HANDLER_HPP_INCLUDED
#define FONT_HANDLER_HPP_INCLUDED

#include <allegro5/allegro_ttf.h>

// (monostate)
class FontHandler
{
private:
    static bool initialized;

public:
    static ALLEGRO_FONT *font;

    FontHandler();
    void Load();
};

#endif // FONT_HANDLER_HPP_INCLUDED
