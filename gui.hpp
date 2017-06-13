#ifndef GUI_HPP_INCLUDED
#define GUI_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <string>

using std::shared_ptr;

class GUI
{
public:
    enum class State
    {
        StartScreen,
        RequestCreateAccount,
        RequestPlayGame,
        CreateAccount,
        Credits,
        Login,
        CharacterList,
        PlayGame
    };

    struct CreateAccount
    {
        char *username;
        char *password;
        char *real_name;
        char *location;
        char *email;

        sf::Clock creation_clock;
        bool approved;
        bool created;

        CreateAccount(char *username, char *password, char *real_name, char *location, char *email);
    };

    struct CharacterEntry
    {
        std::string name;
        unsigned int id;
    };

    struct PopupModal
    {
        const char *str_id;
        std::string title;
        std::string message;
        int type;

        PopupModal(const char *str_id, std::string title, std::string message, int type);
    };

private:
    sf::RenderWindow &window;
    sf::Clock clock;
    State state;
    shared_ptr<sf::Sprite> bg;
    std::string version_address;
    bool connection_closed;
    bool initialize_focus;
    shared_ptr<PopupModal> popup_modal;

public:
    shared_ptr<CreateAccount> create_account;

    GUI(sf::RenderWindow &window_);
    void ProcessEvent(sf::Event &event);
    void Update();
    void Process();
    void Draw();
    void Shutdown();
    void Reset();
    void SetState(State state);
    State GetState();
    void Disconnected();

    void StartScreen();
    void AccountCreation();
    std::vector<std::string> GetAccountInfo();
    void ResetCreateAcc();
    void DisplayPopupModal();
    void CharacterList();
    void GameWindow();
};

#endif // GUI_HPP_INCLUDED
