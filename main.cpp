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

    s.config = shared_ptr<Config>(new Config("./config.ini"));
    s.eif = shared_ptr<EIF>(new EIF("./pub/dat001.eif"));
    s.enf = shared_ptr<ENF>(new ENF("./pub/dtn001.enf"));
    s.esf = shared_ptr<ESF>(new ESF("./pub/dsl001.esf"));
    s.ecf = shared_ptr<ECF>(new ECF("./pub/dat001.ecf"));

    puts("data handlers initialized");
}

int main()
{
    S &s = S::GetInstance();

    initialize_data_handlers();

    sf::RenderWindow window(sf::VideoMode(640, 480), "Endless Online Awaken", sf::Style::Close);
    window.setFramerateLimit(60);

    s.eoclient = shared_ptr<EOClient>(new EOClient());
    s.gfx_loader = shared_ptr<GFXLoader>(new GFXLoader());
    s.gui = shared_ptr<GUI>(new GUI(window));

    while (window.isOpen())
    {
        if(s.eoclient.get())
        {
            bool was_connected = s.eoclient->Connected();

            s.eoclient->Tick();

            if(was_connected && !s.eoclient->Connected())
            {
                s.gui->Disconnected();
            }
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            s.gui->ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                s.call_exit = true;
            }
        }

        s.gui->Update();
        s.gui->Process();

        //ImGui::SetNextWindowFocus();
        //ImGui::ShowTestWindow();

        if(s.call_exit)
        {
            if(s.eoclient->Connected())
            {
                s.eoclient->Disconnect();
            }

            window.close();
        }

        window.clear();

        s.gui->Draw();

        window.display();
    }

    s.gui->Shutdown();

    return 0;
}
