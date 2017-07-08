// Endless Online Awaken v0.0.1

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

shared_ptr<sf::Image> GFXLoader::GFXResource::NewImage(sf::Image &image, int id)
{
    if(!this->GetImage(id).get())
    {
        this->images[id] = shared_ptr<sf::Image>(new sf::Image(image));
    }

    return this->images[id];
}

shared_ptr<sf::Image> GFXLoader::GFXResource::GetImage(int id)
{
    std::unordered_map<int, std::shared_ptr<sf::Image>>::iterator it;
    it = this->images.find(id);

    if(it != this->images.end())
    {
        return this->images[id];
    }
    else
    {
        return shared_ptr<sf::Image>(0);
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

shared_ptr<sf::Image> GFXLoader::LoadImage(int group_id, int id, bool mask)
{
    shared_ptr<sf::Image> image = this->resources[group_id]->GetImage(id);
    if(!image.get())
    {
        sf::Image new_image;
        std::string filename = "./gfx/";
        filename += std::to_string(group_id) + "/" + std::to_string(id + 100) + ".bmp";
        bool loaded = new_image.loadFromFile(filename);

        if(loaded)
        {
            if(mask) new_image.createMaskFromColor(sf::Color(0, 0, 0), 0);
            image = this->resources[group_id]->NewImage(new_image, id);
        }
        else
        {
            printf("GFXLoader: failed to load image %s\n", filename.c_str());
        }
    }

    return image;
}

shared_ptr<sf::Texture> GFXLoader::LoadTexture(int group_id, int id, bool mask)
{
    shared_ptr<GFXResource> resource = this->GetResource(group_id);

    if(!resource.get())
    {
        this->resources[group_id] = shared_ptr<GFXResource>(new GFXResource());
    }

    shared_ptr<sf::Texture> texture = this->resources[group_id]->GetTexture(id);
    if(!texture.get())
    {
        shared_ptr<sf::Image> image = this->LoadImage(group_id, id, mask);

        if(image.get())
        {
            sf::Texture new_texture;
            new_texture.loadFromImage(*image);
            texture = this->resources[group_id]->NewTexture(new_texture, id);
        }
        else
        {
            printf("GFXLoader: failed to load texture %i_%i\n", group_id, id);
        }
    }

    return texture;
}
