// Endless Online Awaken v0.0.1

#include "map.hpp"
#include "singleton.hpp"
#include "util.hpp"

Atlas::Atlas(unsigned int gfx_group_id, unsigned int image_limit)
{
    this->Load(gfx_group_id, image_limit);
}

void Atlas::Load(unsigned int gfx_group_id, unsigned int image_limit)
{
    S &s = S::GetInstance();

    sf::Image image;
    unsigned int tmax = sf::Texture::getMaximumSize();
    tmax = 640 * 480;
    printf("max texture size: %i\n", tmax);
    image.create(64, 32);
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int pixels_used = 0;
    for(unsigned int i = 0; i < image_limit; ++i)
    {
        sf::Image new_image;
        std::string filename = "./gfx/";
        filename += std::to_string(gfx_group_id) + "/" + std::to_string(i + 100) + ".bmp";
        bool loaded = new_image.loadFromFile(filename);
        sf::Vector2u img_size = new_image.getSize();
        if(loaded && x * y + img_size.x + img_size.y < tmax)
        {
            GraphicInfo graphic_info(i, sf::IntRect(x, y, x + img_size.x, y + img_size.y));
            image.copy(new_image, x, y);

            x += img_size.x;

            if(x + img_size.x > tmax / 2)
            {
                x = 0;
                y += img_size.y;
            }

            pixels_used = x * y + img_size.x + img_size.y;
        }
    }

    this->texture.loadFromImage(image);
}

sf::IntRect Atlas::GetImageRect(unsigned int id)
{
    for(unsigned int i = 0; i < this->graphic_info.size(); ++i)
    {
        if(this->graphic_info[i].id == id)
        {
            return this->graphic_info[i].rect;
        }
    }

    return sf::IntRect(0, 0, 0, 0);
}

Map::GraphicObject::GraphicObject(sf::Sprite sprite, int gfx_group_id, int gfx_id)
{
    this->m_sprite = sprite;
    const sf::Texture *texture = this->m_sprite.getTexture();
    this->m_texture = (sf::Texture *)texture;
    this->gfx_group_id = gfx_group_id;
    this->gfx_id = gfx_id;
    this->render_states.texture = this->m_texture;
    this->m_vertices.setPrimitiveType(sf::Quads);
    this->m_vertices.resize(4);
    this->Load();
}

void Map::GraphicObject::Load()
{
    //this->m_vertices.clear();
    //this->m_vertices.resize(4);

    sf::Vertex *quad = &this->m_vertices[0];

    sf::Vector2f position = this->m_sprite.getPosition();
    sf::IntRect rect = this->m_sprite.getTextureRect();
    this->render_states.texture = this->m_texture;

    sf::IntRect tsize = this->m_sprite.getTextureRect();
    quad[0].position = sf::Vector2f(position.x, position.y);
    quad[1].position = sf::Vector2f(position.x + tsize.width, position.y);
    quad[2].position = sf::Vector2f(position.x + tsize.width, position.y + tsize.height);
    quad[3].position = sf::Vector2f(position.x, position.y + tsize.height);

    quad[0].texCoords = sf::Vector2f(rect.left, rect.top);
    quad[1].texCoords = sf::Vector2f(rect.left + rect.width, rect.top);
    quad[2].texCoords = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
    quad[3].texCoords = sf::Vector2f(rect.left, rect.top + rect.height);
}

void Map::GraphicObject::Draw()
{
    S &s = S::GetInstance();

    this->Load();
    s.window.draw(*this);
}

Map::Map()
{

}

void Map::Load()
{
    // construct everything needed for map render
}

void Map::Reset()
{
    this->characters.clear();
    this->npcs.clear();
}

void Map::Process()
{
    for(unsigned int i = 0; i < this->characters.size(); ++i)
    {
        Character *character = &this->characters[i];
        if(character->name == S::GetInstance().character.name)
        {
            character = &S::GetInstance().character;
        }

        character->Process();
    }
}

void Map::Draw()
{
    S &s = S::GetInstance();

    int width = s.emf->width;
    int height = s.emf->height;

    int gfx_id[6] = { 3, 4, 5, 6, 6, 7 };
    int g_xoff[6] = { 0, 0, 0, 0, 0, 0 };
    int g_yoff[6] = { 0, 0, 0, 0, 0, 0 };

    int min_x = s.character.x - 26 >= 0? s.character.x - 26 : 0;
    int min_y = s.character.y - 26 >= 0? s.character.y - 26 : 0;
    int max_x = s.character.x + 26 < width? s.character.x + 26 : width;
    int max_y = s.character.y + 26 < height? s.character.y + 26 : height;

    int cursor_closest_dist = 1000;
    int cursor_closest_dist_x = 0;
    int cursor_closest_dist_y = 0;
    int cursor_closest_dist_draw_x = 0;
    int cursor_closest_dist_draw_y = 0;

    for(int y = min_y; y < max_y; ++y)
    {
        for(int x = min_x; x < max_x; ++x)
        {
            int graphic_id = s.emf->fill_tile;
            if(graphic_id != 0)
            {
                int char_xoff = 0;
                int char_yoff = 0;
                if(s.character.direction == Direction::Down)
                {
                    char_xoff = -s.character.xoff;
                    char_yoff = s.character.yoff;
                }
                else if(s.character.direction == Direction::Left)
                {
                    char_xoff = -s.character.xoff;
                    char_yoff = -s.character.yoff;
                }
                else if(s.character.direction == Direction::Up)
                {
                    char_xoff = s.character.xoff;
                    char_yoff = -s.character.yoff;
                }
                else if(s.character.direction == Direction::Right)
                {
                    char_xoff = s.character.xoff;
                    char_yoff = s.character.yoff;
                }

                int player_x = s.character.x * 64 - s.character.x * 32 - s.character.y * 32;
                int player_y = s.character.y * 32 + s.character.x * 16 - s.character.y * 16;
                int screen_x = 640 / 2 - 32 - player_x;
                int screen_y = 480 / 2 - 480 / 4 + 16 - player_y;

                screen_x += char_xoff;
                screen_y += char_yoff;

                int xoff = x * 64;
                int yoff = y * 32;

                xoff -= x * 32;
                yoff += x * 16;
                xoff -= y * 32;
                yoff -= y * 16;

                int index = this->GetObjectIndex(gfx_id[0], s.emf->fill_tile);
                if(index == -1)
                {
                    this->LoadObject(gfx_id[0], s.emf->fill_tile);
                    index = this->objects.size() - 1;
                }

                this->objects[index].m_sprite.setPosition(screen_x + xoff + g_xoff[0], screen_y + yoff + g_yoff[0]);
                this->objects[index].Draw();

                sf::Vector2i pos = sf::Mouse::getPosition(s.window);
                int distance = path_length(screen_x + xoff + 32, screen_y + yoff + 16, pos.x, pos.y);
                if(distance < cursor_closest_dist)
                {
                    cursor_closest_dist = distance;
                    cursor_closest_dist_x = x;
                    cursor_closest_dist_y = y;
                    cursor_closest_dist_draw_x = screen_x + xoff;
                    cursor_closest_dist_draw_y = screen_y + yoff;
                }
            }
        }
    }

    if(cursor_closest_dist_x != 0)
    {
        int index = this->GetObjectIndex(2, 24);
        if(index == -1)
        {
            this->LoadObject(2, 24);
            index = this->GetObjectIndex(2, 24);
            this->objects[index].m_sprite.setTextureRect(sf::IntRect(0, 0, 64, 32));
        }

        this->objects[index].m_sprite.setPosition(cursor_closest_dist_draw_x, cursor_closest_dist_draw_y);
        this->objects[index].Draw();
    }

    for(int y = min_y; y < max_y; ++y)
    {
        for(int x = min_x; x < max_x; ++x)
        {
            for(int l = 0; l < 6; ++l)
            {
                int graphic_id = s.emf->GetGraphicID(l, x, y);

                int char_xoff = 0;
                int char_yoff = 0;
                if(s.character.direction == Direction::Down)
                {
                    char_xoff = -s.character.xoff;
                    char_yoff = s.character.yoff;
                }
                else if(s.character.direction == Direction::Left)
                {
                    char_xoff = -s.character.xoff;
                    char_yoff = -s.character.yoff;
                }
                else if(s.character.direction == Direction::Up)
                {
                    char_xoff = s.character.xoff;
                    char_yoff = -s.character.yoff;
                }
                else if(s.character.direction == Direction::Right)
                {
                    char_xoff = s.character.xoff;
                    char_yoff = s.character.yoff;
                }

                int player_x = s.character.x * 64 - s.character.x * 32 - s.character.y * 32;
                int player_y = s.character.y * 32 + s.character.x * 16 - s.character.y * 16;
                int screen_x = 640 / 2 - 32 - player_x;
                int screen_y = 480 / 2 - 480/ 4 + 16- player_y;

                screen_x += char_xoff;
                screen_y += char_yoff;

                int xoff = x * 64;
                int yoff = y * 32;

                xoff -= x * 32;
                yoff += x * 16;
                xoff -= y * 32;
                yoff -= y * 16;

                if(graphic_id != 0)
                {
                    int index = this->GetObjectIndex(gfx_id[l], graphic_id);
                    if(index == -1)
                    {
                        this->LoadObject(gfx_id[l], graphic_id);
                        index = this->GetObjectIndex(gfx_id[l], graphic_id);
                    }

                    const sf::Texture *tex1 = this->objects[index].m_sprite.getTexture();

                    if(l == 1) // objects
                    {
                        g_xoff[l] = 32 - tex1->getSize().x / 2;
                        g_yoff[l] = 0 - (tex1->getSize().y - 32);
                    }
                    else if(l == 3) // wall down
                    {
                        g_xoff[l] = -16 + tex1->getSize().x / 2;
                        g_yoff[l] = -tex1->getSize().y + 32;
                    }
                    else if(l == 4) // wall right
                    {
                        g_xoff[l] = 16 + tex1->getSize().x / 2;
                        g_yoff[l] = -tex1->getSize().y + 32;
                    }

                    index = this->GetObjectIndex(gfx_id[l], graphic_id);

                    this->objects[index].m_sprite.setPosition(screen_x + xoff + g_xoff[l], screen_y + yoff + g_yoff[l]);

                    this->objects[index].Draw();

                    if(l == 0 && cursor_closest_dist_x != 0 && x == cursor_closest_dist_x && y == cursor_closest_dist_y)
                    {
                        int index = this->GetObjectIndex(2, 24);
                        if(index == -1)
                        {
                            this->LoadObject(2, 24);
                            index = this->GetObjectIndex(2, 24);
                            this->objects[index].m_sprite.setTextureRect(sf::IntRect(0, 0, 64, 32));

                        }

                        this->objects[index].m_sprite.setPosition(cursor_closest_dist_draw_x, cursor_closest_dist_draw_y);
                        this->objects[index].Draw();
                    }
                }

                if(l == 0)
                {
                    for(unsigned int i = 0; i < this->characters.size(); ++i)
                    {
                        if(this->characters[i].x == x && this->characters[i].y == y && this->characters[i].gameworld_id != s.character.gameworld_id)
                        {
                            this->characters[i].Draw(screen_x + xoff, screen_y + yoff);
                        }
                    }
                }

                if(l == 0 && x == s.character.x && y == s.character.y)
                {
                    s.character.Draw(screen_x + xoff, screen_y + yoff);
                }
            }
        }
    }
}

int Map::GetCharacterIndex(short gameworld_id)
{
    for(std::size_t i = 0; i < this->characters.size(); ++i)
    {
        if(this->characters[i].gameworld_id == gameworld_id)
        {
            return i;
        }
    }

    return -1;
}

int Map::GetCharacterIndex(std::string name)
{
    for(std::size_t i = 0; i < this->characters.size(); ++i)
    {
        if(this->characters[i].name == name)
        {
            return i;
        }
    }

    return -1;
}

void Map::RemoveCharacter(short gameworld_id)
{
    int i = this->GetCharacterIndex(gameworld_id);
    if(i != -1)
    {
        this->characters.erase(this->characters.begin() + i);
    }
}

int Map::GetNPCIndex(unsigned char gameworld_index)
{
    for(std::size_t i = 0; i < this->npcs.size(); ++i)
    {
        if(this->npcs[i].gameworld_index == gameworld_index)
        {
            return i;
        }
    }

    return -1;
}

void Map::RemoveNPC(unsigned char gameworld_index)
{
    int i = this->GetNPCIndex(gameworld_index);
    if(i != -1)
    {
        this->npcs.erase(this->npcs.begin() + i);
    }
}

bool Map::Occupied(unsigned char x, unsigned char y)
{
    for(unsigned int i = 0; i < this->characters.size(); ++i)
    {
        if(this->characters[i].x == x && this->characters[i].y == y)
        {
            return true;
        }
    }

    for(unsigned int i = 0; i < this->npcs.size(); ++i)
    {
        if(this->npcs[i].x == x && this->npcs[i].y == y)
        {
            return true;
        }
    }

    return false;
}

bool Map::Walkable(unsigned char x, unsigned char y)
{
    S &s = S::GetInstance();

    if(s.emf->Walkable(x, y) && !s.map.Occupied(x, y))
    {
        return true;
    }

    return false;
}

int Map::GetObjectIndex(int gfx_group_id, int gfx_id)
{
    for(unsigned int i = 0; i < this->objects.size(); ++i)
    {
        if(this->objects[i].gfx_group_id == gfx_group_id && this->objects[i].gfx_id == gfx_id)
        {
            return i;
        }
    }

    return -1;
}

void Map::LoadObject(int gfx_group_id, int gfx_id)
{
    S &s = S::GetInstance();

    shared_ptr<sf::Texture> tex = s.gfx_loader.LoadTexture(gfx_group_id, gfx_id);
    s.map.objects.push_back(GraphicObject(sf::Sprite(*tex), gfx_group_id, gfx_id));
}
