// Endless Online Awaken

#ifndef GUI_HPP_INCLUDED
#define GUI_HPP_INCLUDED

#include "gfx_loader.hpp"

#include <allegro5/allegro5.h>
#include <memory>
#include <vector>

// to put all gui forms together and process it / render (monostate)
class GUI
{
public:
    enum class State
    {
        MainMenu,
        Editor,
        CreateAccount,
        Credits,
        CharacterList,
        CreateCharacter,
        Playing
    };

private:
    static bool initialized_;
    static State state;
    static unsigned int bg;
    static std::string open_popup;
    static std::string popup_message;
    static int input_focus;
    static bool clear_gfx;

public:
    GUI();
    void Process();
    void Render();
    void SetState(State state);
    State GetState();
    void OpenPopup(std::string id, std::string message = "");
    void SetFocus(int widget);

    unsigned int BackgroundID() { return this->bg; }
    void MainMenu();
    void CreateAccount();
    void Login(bool *p_open);
    void Credits();
    void CharacterList();
    void CreateCharacter();
    void Playing();
    void ChatBox();
};

class Chat
{
private:
    static bool initialized_;

public:
    static std::vector<std::pair<std::string, std::string>> messages;
    static bool new_message;

    Chat();
    void AddMessage(std::string char_name, std::string message);
};

#endif // GUI_HPP_INCLUDED
