#include "map_cursor.hpp"

#include "gfx_loader.hpp"
#include "map.hpp"
#include "client.hpp"

#include <iostream>

bool MapCursor::initialized_ = false;
int MapCursor::rx;
int MapCursor::ry;
std::string MapCursor::nickname;
unsigned short MapCursor::x;
unsigned short MapCursor::y;

MapCursor::MapCursor()
{
    if(!this->initialized_)
    {
        this->rx = 0;
        this->ry = 0;
        this->nickname = "";
        this->x = 0;
        this->y = 0;

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

    int mx = m_state.x;
    int my = m_state.y;

    this->rx = rx;
    this->ry = ry;

    bool update = false;

    Character *character = Client().character;
    unsigned short start_x = 0;
    unsigned short start_y = 0;
    unsigned short end_x = map.width > 0? map.width - 1 : 1;
    unsigned short end_y = map.height > 0? map.height - 1 : 1;

    if(character != 0)
    {
        start_x = character->x > 16? character->x - 16 : 0;
        start_y = character->y > 16? character->y - 16 : 0;
        end_x = character->x + 16 > map.width? map.width - 1 : character->x + 16;
        end_y = character->y + 16 > map.height? map.height - 1 : character->y + 16;
    }

    for(unsigned short x = start_x; x <= end_x; ++x)
    {
        for(unsigned short y = start_y; y <= end_y; ++y)
        {
            if(!this->TestMousePos(mx, my, x, y)) continue;

            this->x = x;
            this->y = y;
            update = true;

            break;
        }

        if(update) break;
    }

    int screen_x = this->x * 64 - this->x * 32 - this->y * 32 + this->rx;
    int screen_y = this->y * 16 + this->x * 16 + this->ry;

    al_draw_bitmap_region(bitmap, 0, 0, 64, 32, screen_x, screen_y, 0);

    std::vector<Character *> chars = Map().GetCharactersAt(this->x, this->y);
    if(chars.size() > 0)
    {
        this->nickname = chars[0]->name;
    }
    else
    {
        this->nickname = "";
    }
}

bool MapCursor::TestMousePos(int mx, int my, unsigned short x, unsigned short y)
{
    int screen_x = x * 64 - x * 32 - y * 32 + this->rx;
    int screen_y = y * 16 + x * 16 + this->ry;

    bool test = (mx >= screen_x + 16 && mx < screen_x + 48 && my >= screen_y + 8 && my < screen_y + 24);

    if(!test)
        test = (mx >= screen_x + 2 && mx < screen_x + 62 && my >= screen_y + 14 && my < screen_y + 18);

    if(!test)
        test = (mx >= screen_x + 26 && mx < screen_x + 38 && my >= screen_y + 2 && my < screen_y + 30);

    return test;
}

bool MapCursor::MouseOnMap()
{
    ALLEGRO_MOUSE_STATE m_state;
    al_get_mouse_state(&m_state);

    int mx = m_state.x;
    int my = m_state.y;

    Map map;

    Character *character = Client().character;
    unsigned short start_x = 0;
    unsigned short start_y = 0;
    unsigned short end_x = map.width > 0? map.width - 1 : 1;
    unsigned short end_y = map.height > 0? map.height - 1 : 1;

    if(character != 0)
    {
        start_x = character->x > 16? character->x - 16 : 0;
        start_y = character->y > 16? character->y - 16 : 0;
        end_x = character->x + 16 > map.width? map.width - 1 : character->x + 16;
        end_y = character->y + 16 > map.height? map.height - 1 : character->y + 16;
    }

    for(unsigned short x = start_x; x <= end_x; ++x)
    {
        for(unsigned short y = start_y; y <= end_y; ++y)
        {
            int screen_x = x * 64 - x * 32 - y * 32 + this->rx;
            int screen_y = y * 16 + x * 16 + this->ry;

            bool test = (mx >= screen_x + 16 && mx < screen_x + 48 && my >= screen_y + 8 && my < screen_y + 24);

            if(!test)
                test = (mx >= screen_x && mx < screen_x + 64 && my >= screen_y + 14 && my < screen_y + 18);

            if(!test)
                test = (mx >= screen_x + 26 && mx < screen_x + 38 && my >= screen_y && my < screen_y + 32);

            if(test) return true;
        }
    }

    return false;
}
