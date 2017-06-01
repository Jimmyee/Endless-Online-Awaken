#ifndef GUI_HPP_INCLUDED
#define GUI_HPP_INCLUDED

#include <TGUI/TGUI.hpp>

class GUI
{
private:
    tgui::Gui tgui;

public:
    GUI(sf::RenderWindow& window);
    void HandleEvent(sf::Event &event);
    void Draw();
};

#endif // GUI_HPP_INCLUDED
