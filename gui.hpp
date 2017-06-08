#ifndef GUI_HPP_INCLUDED
#define GUI_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <memory>

using std::shared_ptr;

class GUI
{
public:
    enum class State
    {
        StartScreen,
        CreateAccount,
        PlayGame,
        Credits,
        Login,
        CharacterList,
    };

private:
    sf::RenderWindow &window;
    sf::Clock clock;
    State state;
    int child_window;
    shared_ptr<sf::Sprite> bg;
    std::string version_address;
    bool connection_closed;

public:
    GUI(sf::RenderWindow &window_);
    void ProcessEvent(sf::Event &event);
    void Update();
    void Process();
    void Draw();
    void Shutdown();
    void Reset();
    State GetState();
    void Disconnected();

    void StartScreen();
};

#endif // GUI_HPP_INCLUDED
