// Endless Online Awaken v0.0.1

#ifndef GUI_HPP_INCLUDED
#define GUI_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <list>
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
        std::string username;
        std::string password;
        std::string real_name;
        std::string location;
        std::string email;

        sf::Clock creation_clock;
        bool approved;
        bool created;

        CreateAccount();
    };

    struct PopupModal
    {
        const char *str_id;
        std::string title;
        std::string message;
        int type;
        bool open;
        bool need_open;

        PopupModal(const char *str_id, std::string title, std::string message, int type);
    };

    struct TextField
    {
        std::string text;
        std::size_t min_len;
        std::size_t max_len;

        TextField(std::string text, std::size_t min_len, std::size_t max_len);
        std::string GetText();
        bool ValidateLength();
    };

    struct ChatConsole
    {
        struct ChatMessage
        {
            std::string name;
            std::string message;

            ChatMessage(std::string name, std::string message);
        };

        std::vector<ChatMessage> buffer;
        std::size_t last_buffer_size;

        ChatConsole();
        void AddMessage(ChatMessage message);
        void Draw();
    };

private:
    sf::RenderWindow &window;
    sf::Clock clock;
    State state;
    shared_ptr<sf::Sprite> bg;
    shared_ptr<sf::Sprite> bg_avatar;
    std::string version_address;
    bool initialize_focus;

public:
    shared_ptr<PopupModal> popup_modal;
    shared_ptr<CreateAccount> create_account;
    std::vector<TextField> text_fields;
    ChatConsole chat_console;

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

    void DisplayPopupModal();

    void StartScreen();
    void AccountCreation();
    void CharacterList();
    void GameWindow();
};

#endif // GUI_HPP_INCLUDED
