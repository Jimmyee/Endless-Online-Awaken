#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include "config.hpp"
#include "eoclient.hpp"
#include "eodata.hpp"

#include <memory>

using std::shared_ptr;

class Game
{
public:
    Config config;
    EOClient eoclient;
    shared_ptr<EMF> emf;
    shared_ptr<EIF> eif;
    shared_ptr<ENF> enf;
    shared_ptr<ESF> esf;
    shared_ptr<ECF> ecf;
    bool loaded;

    Game();
    void Load(std::string filename);
    void Tick();
    void Reset();
};


#endif // GAME_HPP_INCLUDED
