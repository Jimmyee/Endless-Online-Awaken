// Endless Online Awaken

#include "gfx_loader.hpp"

#include <iostream>

bool GFXLoader::initialized_ = false;
std::map<unsigned int, std::unique_ptr<GFXLoader::Directory>> GFXLoader::directories;
std::string GFXLoader::path;

GFXLoader::GFXLoader()
{
    if(!this->initialized_)
    {
        this->path = "";

        this->initialized_ = true;
    }
}

GFXLoader::GFXLoader(std::string path)
{
    this->path = path;

    if(!this->initialized_)
    {
        this->initialized_ = true;
    }
}

GFXLoader::Directory::Directory(std::string path, unsigned int id)
{
    this->path = path;
    this->id = id;
}

ALLEGRO_BITMAP *GFXLoader::Directory::GetBitmap(unsigned int id, bool mask, std::string format)
{
    if(this->bitmaps.find(id) == this->bitmaps.end())
    {
        if(format == "") format = ".bmp";
        std::string filename = path + "/" + std::to_string(this->id) + "/" + std::to_string(id + 100) + format;
        this->bitmaps[id] = al_load_bitmap(filename.c_str());

        if(!this->bitmaps[id])
        {
            return NULL;
            //throw std::runtime_error("Can't load " + filename);
        }
        else
        {
            if(mask)
            {
                al_convert_mask_to_alpha(this->bitmaps[id], al_map_rgb(0, 0, 0));
            }
        }
    }

    return this->bitmaps[id];
}

void GFXLoader::Directory::Clear()
{
    for(auto &it : this->bitmaps)
    {
        al_destroy_bitmap(it.second);
    }

    this->bitmaps.clear();
}

ALLEGRO_BITMAP *GFXLoader::GetBitmap(unsigned int dir_id, unsigned int bmp_id, bool mask, std::string format)
{
    if(this->directories.find(dir_id) == this->directories.end())
    {
        this->directories[dir_id] = std::make_unique<Directory>(this->path, dir_id);
    }

    return this->directories[dir_id]->GetBitmap(bmp_id, mask, format);
}

void GFXLoader::Clear()
{
    for(auto &it : this->directories)
    {
        it.second->Clear();
    }
}
