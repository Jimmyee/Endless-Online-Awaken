// Endless Online Awaken v0.0.1

#ifndef GFXLOADER_HPP_INCLUDED
#define GFXLOADER_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>

using std::shared_ptr;
using std::unique_ptr;

class GFXLoader
{
public:
    struct GFXResource
    {
        std::unordered_map<int, shared_ptr<sf::Image>> images;
        std::unordered_map<int, shared_ptr<sf::Texture>> textures;

        shared_ptr<sf::Texture> NewTexture(sf::Texture &texture, int id);
        shared_ptr<sf::Texture> GetTexture(int id);
        shared_ptr<sf::Image> NewImage(sf::Image &image, int id);
        shared_ptr<sf::Image> GetImage(int id);
    };

private:
    std::unordered_map<int, shared_ptr<GFXResource>> resources;

    shared_ptr<GFXResource> GetResource(int id);

public:
    shared_ptr<sf::Image> LoadImage(int group_id, int id, bool mask = true);
    shared_ptr<sf::Texture> LoadTexture(int group_id, int id, bool mask = true);
};

#endif // GFXLOADER_HPP_INCLUDED
