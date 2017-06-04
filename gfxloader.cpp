#include "gfxloader.hpp"
#include <string>

shared_ptr<sf::Texture> GFXLoader::GFXResource::NewTexture(sf::Texture &texture, int id)
{
    if(!this->GetTexture(id).get())
    {
        this->textures[id] = shared_ptr<sf::Texture>(new sf::Texture(texture));
    }

    return this->textures[id];
}

shared_ptr<sf::Texture> GFXLoader::GFXResource::GetTexture(int id)
{
    std::unordered_map<int, std::shared_ptr<sf::Texture>>::iterator it;
    it = this->textures.find(id);

    if(it != this->textures.end())
    {
        return this->textures[id];
    }
    else
    {
        return shared_ptr<sf::Texture>(0);
    }
}

shared_ptr<GFXLoader::GFXResource> GFXLoader::GetResource(int id)
{
    std::unordered_map<int, shared_ptr<GFXResource>>::iterator it;
    it = this->resources.find(id);
    if(it != this->resources.end())
    {
        return this->resources[id];
    }
    else
    {
        return shared_ptr<GFXResource>(0);
    }
}

shared_ptr<sf::Texture> GFXLoader::LoadTexture(int gfx_id, int tex_id)
{
    shared_ptr<GFXResource> resource = this->GetResource(gfx_id);

    if(!resource.get())
    {
        this->resources[gfx_id] = shared_ptr<GFXResource>(new GFXResource());
    }

    shared_ptr<sf::Texture> texture = this->resources[gfx_id]->GetTexture(tex_id);
    if(!texture.get())
    {
        sf::Image image;
        sf::Texture new_texture;
        std::string filename = "./gfx/";
        filename += std::to_string(gfx_id) + "/" + std::to_string(tex_id + 100) + ".bmp";
        bool loaded = image.loadFromFile(filename);

        if(loaded)
        {
            image.createMaskFromColor(sf::Color(0, 0, 0), 0);
            new_texture.loadFromImage(image);
            texture = this->resources[gfx_id]->NewTexture(new_texture, tex_id);
        }
        else
        {
            printf("GFXLoader: failed to load %s\n", filename.c_str());
        }
    }

    return texture;
}
