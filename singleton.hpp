// Endless Online Awaken v0.0.1

#ifndef SINGLETON_HPP_INCLUDED
#define SINGLETON_HPP_INCLUDED

#include "randgen.hpp"
#include "gfxloader.hpp"
#include "gui.hpp"
#include "inputhandler.hpp"
#include "eoclient.hpp"
#include "config.hpp"
#include "eventprocessor.hpp"
#include "eodata.hpp"
#include "map.hpp"
#include "inventory.hpp"

#include <memory>
#include <vector>

using std::shared_ptr;

class S
{
public:
    static S& GetInstance()
    {
        static S    instance;
        return instance;
    }
private:
    S() { call_exit = false; }
public:
    S(S const&)               = delete;
    void operator=(S const&)  = delete;

    bool call_exit;

    RandomGenerator rand_gen;
    sf::RenderWindow window;
    GFXLoader gfx_loader;
    GUI gui;
    InputHandler input_handler;
    EOClient eoclient;
    Config config;
    EventProcessor eprocessor;
    sf::Clock init_clock;

    shared_ptr<EMF> emf;
    shared_ptr<EIF> eif;
    shared_ptr<ENF> enf;
    shared_ptr<ESF> esf;
    shared_ptr<ECF> ecf;

    Map map;
    Character character;
    Inventory inventory;
};

#endif // SINGLETON_HPP_INCLUDED
