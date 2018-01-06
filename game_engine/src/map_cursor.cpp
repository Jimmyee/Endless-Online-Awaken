#include "map_cursor.hpp"

#include "gfx_loader.hpp"
#include "map.hpp"

#include <iostream>

bool MapCursor::initialized_ = false;
std::string MapCursor::nickname;

MapCursor::MapCursor()
{
    if(!this->initialized_)
    {
        this->nickname = "";

        this->initialized_ = true;
    }
}

void MapCursor::Render(int rx, int ry)
{
    ALLEGRO_BITMAP *bitmap = GFXLoader().GetBitmap(2, 24);

    if(bitmap == NULL) return;

    Map map;

    ALLEGRO_MOUSE_STATE m_state;
    al_get_mouse_state(&m_state);

    int m_x = m_state.x;
    int m_y = m_state.y;

    bool draw = true;

    for(int x = 0; x < map.width; ++x)
    {
        for(int y = 0; y < map.height; ++y)
        {
            int screen_x = x * 64 - x * 32 - y * 32 + rx;
            int screen_y = y * 16 + x * 16 + ry;

            if(m_x + 8 >= screen_x && m_x + 8 < screen_x + 64 && m_y + 4 >= screen_y && m_y + 8 < screen_y + 32)
            {
                al_draw_bitmap_region(bitmap, 0, 0, 64, 32, screen_x, screen_y, 0);
                draw = false;

                std::vector<Character *> chars = Map().GetCharactersAt(x, y);
                if(chars.size() > 0)
                {
                    this->nickname = chars[0]->name;
                }
                else
                {
                    this->nickname = "";
                }

                break;
            }
        }

        if(!draw) break;
    }
}
