#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include "singleton.hpp"

void initialize_data()
{
    S &s = S::GetInstance();

    s.config = shared_ptr<Config>(new Config("./config.ini"));
    s.eif = shared_ptr<EIF>(new EIF("./pub/dat001.eif"));
    s.enf = shared_ptr<ENF>(new ENF("./pub/dtn001.enf"));
    s.esf = shared_ptr<ESF>(new ESF("./pub/dsl001.esf"));
    s.ecf = shared_ptr<ECF>(new ECF("./pub/dat001.ecf"));
    s.eoclient = shared_ptr<EOClient>(new EOClient());

    puts("Data files initialized");
}

int main()
{
    S &s = S::GetInstance();
    initialize_data();

    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML works!");

    if(s.eoclient->Connect())
    {
        s.eoclient->RequestInit();
    }

    while (window.isOpen())
    {
        if(s.eoclient.get())
        {
            s.eoclient->Tick();
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if(s.gui.get())
            {
                s.gui->HandleEvent(event);
            }
        }

        window.clear();

        if(s.gui.get())
        {
            s.gui->Draw();
        }

        window.display();
    }

    return 0;
}
