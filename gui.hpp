#ifndef GUI_HPP_INCLUDED
#define GUI_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <memory>

using std::shared_ptr;

class GUI
{
private:
    sf::RenderWindow &window;
    sf::Clock clock;
    shared_ptr<sf::Sprite> bg;
    std::string version_address;

public:
    GUI(sf::RenderWindow &window_);
    ~GUI();
    void ProcessEvent(sf::Event &event);
    void Update();
    void Process();
    void Draw();

    void StartScreen();
};

#endif // GUI_HPP_INCLUDED
