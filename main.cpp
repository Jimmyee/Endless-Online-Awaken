#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include "config.hpp"
#include "eoclient.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML works!");
    tgui::Gui gui{window};

    Config config;
    config.Load("config.ini");
    EOClient eoclient(config);

    if(eoclient.Connect())
    {
        eoclient.RequestInit();
    }

    while (window.isOpen())
    {
        eoclient.Tick();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            gui.handleEvent(event);
        }

        window.clear();

        gui.draw();

        window.display();
    }

    return 0;
}
