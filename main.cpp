// Endless Online Awaken v0.0.1

#include <SFML/Graphics.hpp>
#include <imgui-sfml.h>
#include <imgui.h>
#include <string>
#include <random>
#include <chrono>

#include "singleton.hpp"

// TODO: create an alternative for singleton

void initialize_data_handlers()
{
    S &s = S::GetInstance();

    s.config.Load("./config.ini");
    s.eif = shared_ptr<EIF>(new EIF("./pub/dat001.eif"));
    s.enf = shared_ptr<ENF>(new ENF("./pub/dtn001.enf"));
    s.esf = shared_ptr<ESF>(new ESF("./pub/dsl001.esf"));
    s.ecf = shared_ptr<ECF>(new ECF("./pub/dat001.ecf"));

    puts("data handlers initialized");
}

int main()
{
    initialize_data_handlers();

    S &s = S::GetInstance();

    sf::Uint32 style;
    bool fullscreen = (bool)std::atoi(s.config.GetValue("Fullscreen").c_str());
    if(fullscreen)
    {
        style = sf::Style::Close | sf::Style::Fullscreen;
    }
    else
    {
        style = sf::Style::Close;
    }
    s.window.create(sf::VideoMode(640, 480), "Endless Online Awaken", style);
    s.window.setFramerateLimit(60);

    s.gui.Initialize();

    //Atlas atlas(3, 724);

    while(s.window.isOpen())
    {
        bool was_connected = s.eoclient.Connected();

        if(s.eoclient.Connected())
        {
            if(s.eoclient.GetState() == EOClient::State::Uninitialized && s.init_clock.getElapsedTime().asSeconds() >= 10)
            {
                puts("Initialization time out.");
                s.eoclient.Disconnect();
            }
        }

        s.eoclient.Tick();

        if(was_connected && !s.eoclient.Connected())
        {
            s.gui.Disconnected();
        }

        sf::Event event;
        while(s.window.pollEvent(event))
        {
            s.gui.ProcessEvent(event);

            if(event.type == sf::Event::Closed)
            {
                s.call_exit = true;
            }
            else if(event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
            {
                s.input_handler.Process(event);
            }
        }

        s.gui.Update();
        s.gui.Process();

        s.eprocessor.Process();

        if(s.gui.GetState() == GUI::State::PlayGame)
        {
            s.map.Process();
        }

        //ImGui::SetNextWindowFocus();
        //ImGui::ShowTestWindow();

        if(s.call_exit)
        {
            if(s.eoclient.Connected())
            {
                s.eoclient.Disconnect();
            }

            s.window.close();
        }

        s.window.clear();

        if(s.gui.GetState() == GUI::State::PlayGame)
        {
            s.map.Draw();
        }

        s.gui.Draw();
        //sf::Sprite sprite(atlas.texture);
        //sprite.setPosition(0, 0);
        //s.window.draw(sprite);

        s.window.display();
    }

    s.gui.Shutdown();

    return 0;
}
