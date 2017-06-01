#include "gui.hpp"

GUI::GUI(sf::RenderWindow& window)
: tgui(window)
{

}

void GUI::HandleEvent(sf::Event &event)
{
    this->tgui.handleEvent(event);
}

void GUI::Draw()
{
    this->tgui.draw();
}
