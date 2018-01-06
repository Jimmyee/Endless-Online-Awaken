#include "font_handler.hpp"

#include <iostream>

bool FontHandler::initialized = false;
ALLEGRO_FONT *FontHandler::font;

FontHandler::FontHandler()
{
    if(!this->initialized)
    {
        this->Load();

        this->initialized = true;
    }
}

void FontHandler::Load()
{
    this->font = al_load_ttf_font("data/fonts/micross.ttf", 14, 0);

    if(font == NULL)
    {
        std::cout << "Could not load font" << std::endl;
    }
}
