// Endless Online Awaken

#ifndef GFX_LOADER_HPP_INCLUDED
#define GFX_LOADER_HPP_INCLUDED

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <map>
#include <memory>

// to load and store graphics (monostate)
class GFXLoader
{
public:
    class Directory
    {
    private:
        std::string path;
        unsigned int id;
        std::map<unsigned int, ALLEGRO_BITMAP *> bitmaps;

    public:
        Directory(std::string path, unsigned int id);
        ALLEGRO_BITMAP *GetBitmap(unsigned int id, bool mask = true);
        void Clear();
    };

private:
    static bool initialized_;
    static std::map<unsigned int, std::unique_ptr<Directory>> directories;

public:
    static std::string path;

    GFXLoader();
    GFXLoader(std::string path);
    ALLEGRO_BITMAP *GetBitmap(unsigned int dir_id, unsigned int bmp_id, bool mask = true);
    void Clear();
};

#endif // GFX_LOADER_HPP_INCLUDED
