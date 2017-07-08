// Endless Online Awaken v0.0.1

#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include "eodata.hpp"
#include "character.hpp"
#include "npc.hpp"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

using std::shared_ptr;

struct Atlas
{
    struct GraphicInfo
    {
        unsigned int id;
        sf::IntRect rect;

        GraphicInfo(unsigned int id, sf::IntRect rect) { this->id = id; this->rect = rect;  }
    };

    sf::Texture texture;
    std::vector<GraphicInfo> graphic_info;

    Atlas(unsigned int gfx_group_id, unsigned int image_limit);
    void Load(unsigned int gfx_group_id, unsigned int image_limit);
    sf::IntRect GetImageRect(unsigned int id);
};

// class responsible for rendering the map and holding data about map's objects
class Map
{
public:
    class GraphicObject : public sf::Drawable, public sf::Transformable
    {
    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            states.transform *= getTransform();
            states.texture = this->m_texture;
            target.draw(this->m_vertices, states);
        }

    public:
        int gfx_group_id;
        int gfx_id;
        sf::Sprite m_sprite;
        sf::Texture *m_texture;
        sf::VertexArray m_vertices;
        sf::RenderStates render_states;

        GraphicObject(sf::Sprite sprite, int gfx_group_id, int gfx_id);
        void Load();
        void Draw();
    };

public:
    std::vector<Character> characters;
    std::vector<NPC> npcs;

    std::vector<GraphicObject> objects;

    Map();
    void Load();
    void Reset();
    void Process();
    void Draw();

    int GetCharacterIndex(short gameworld_id);
    int GetCharacterIndex(std::string name);
    void RemoveCharacter(short gameworld_id);
    int GetNPCIndex(unsigned char gameworld_index);
    void RemoveNPC(unsigned char gameworld_index);
    bool Occupied(unsigned char x, unsigned char y);
    bool Walkable(unsigned char x, unsigned char y);
    int GetObjectIndex(int gfx_group_id, int gfx_id);
    void LoadObject(int gfx_group_id, int gfx_id);
};

#endif // MAP_HPP_INCLUDED
