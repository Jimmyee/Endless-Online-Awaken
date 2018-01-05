// Endless Online Awaken

#include "gui.hpp"

#include "util.hpp"
#include "config.hpp"
#include "game_state.hpp"
#include "client.hpp"
#include "map_editor.hpp"
#include "map.hpp"

#include <imgui.h>
#include "imgui_impl_a5.h"
#include <iostream>

bool GUI::initialized_ = false;
GUI::State GUI::state;
unsigned int GUI::bg;
std::string GUI::open_popup;
std::string GUI::popup_message;
int GUI::input_focus;
bool GUI::clear_gfx;

bool Chat::initialized_ = false;
std::vector<std::pair<std::string, std::string>> Chat::messages;
bool Chat::new_message;

GUI::GUI()
{
    if(!this->initialized_)
    {
        this->state = GUI::State::MainMenu;
        this->bg = 0;
        this->open_popup = "";
        this->popup_message = "";
        this->input_focus = 0;
        this->clear_gfx = false;
        this->SetState(State::MainMenu);

        this->initialized_ = true;
    }
}

void GUI::Process()
{
    bool show_test_window = false;

    if(this->state == State::MainMenu)
    {
        this->MainMenu();
    }
    else if(this->state == State::Editor)
    {
        MapEditor().MakeGUI();
    }
    else if(this->state == State::CreateAccount)
    {
        this->CreateAccount();
    }
    else if(this->state == State::Credits)
    {
        this->Credits();
    }
    else if(this->state == State::CharacterList)
    {
        this->CharacterList();
    }
    else if(this->state == State::CreateCharacter)
    {
        this->CreateCharacter();
    }
    else if(this->state == State::Playing)
    {
        this->Playing();
    }

    if (show_test_window)
    {
        ImGui::SetNextWindowPos(ImVec2(20, 10));
        ImGui::ShowTestWindow(&show_test_window);
    }

    if(!this->open_popup.empty())
    {
        ImGui::OpenPopup(this->open_popup.c_str());
        this->open_popup.clear();
    }

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize;

    if(ImGui::BeginPopupModal("Disconnected", NULL, flags))
    {
        ImGui::Text("Connection with the game server has been lost.");
        if(ImGui::Button("OK"))
        {
            this->SetState(State::MainMenu);
            GameState().Set(GameState::State::MainMenu);

            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    if(ImGui::BeginPopupModal("Connection error", NULL, flags))
    {
        ImGui::Text("Could not connect to the game server.");
        if(ImGui::Button("OK"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    if(ImGui::BeginPopupModal("Account created", NULL, flags))
    {
        ImGui::Text("Account created! Now you can log into the game.");
        if(ImGui::Button("OK"))
        {
            this->SetState(State::MainMenu);

            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    if(ImGui::BeginPopupModal("Character created", NULL, flags))
    {
        ImGui::Text("Character created!");
        if(ImGui::Button("OK"))
        {
            this->SetState(State::CharacterList);
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    if(ImGui::BeginPopupModal("Server answer", NULL, flags))
    {
        ImGui::Text(this->popup_message.c_str());
        if(ImGui::Button("OK"))
        {
            this->popup_message.clear();
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void GUI::Render()
{
    bool draw_bg = this->state == State::MainMenu || this->state == State::CreateAccount || this->state == State::CharacterList
    || this->state == State::Credits || this->state == State::CreateCharacter;
    if(draw_bg)
    {
        ALLEGRO_BITMAP *bg = GFXLoader().GetBitmap(1, this->bg);
        if(bg != NULL) al_draw_bitmap(bg, 0, 0, 0);
    }

    if(this->state == State::Playing)
    {
        ALLEGRO_BITMAP *bg = GFXLoader().GetBitmap(2, 1);
        if(bg != NULL) al_draw_bitmap(bg, 0, 0, 0);
    }

    ImGui::Render();

    if(this->clear_gfx)
    {
        GFXLoader().Clear();
        this->clear_gfx = false;
    }
}

void GUI::SetState(State state)
{
    this->clear_gfx = true;
    if(state == State::MainMenu)
    {
        this->bg = RandGen().RandInt(30, 36);
    }

    this->state = state;
}

GUI::State GUI::GetState()
{
    return this->state;
}

void GUI::OpenPopup(std::string id, std::string message)
{
    this->open_popup = id;
    this->popup_message = message;
}

void GUI::SetFocus(int widget)
{
    this->input_focus = widget;
}

void GUI::MainMenu()
{
    static bool show_login = false;
    Config config;
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowPos(ImVec2(20.f, 200.f));
    if(!ImGui::Begin("Main menu", NULL, flags))
    {
        ImGui::End();
        return;
    }

    std::string action = "";
    if(ImGui::Button("Create account"))
    {
        action = "create account";
    }
    if(ImGui::Button("Play game"))
    {
        action = "play game";
    }
    if(ImGui::Button("Credits"))
    {
        action = "credits";
    }
    if(ImGui::Button("Exit"))
    {
        GameState().Set(GameState::State::Exit);
    }

    if(action == "create account")
    {
        Client client;

        if(!client.Connected())
        {
            if(!client.Connect(config.GetValue("Address"), std::atoi(config.GetValue("Port").c_str())))
                this->OpenPopup("Connection error");
        }

        if(client.Connected())
        {
            this->SetState(State::CreateAccount);
            this->SetFocus(1);
        }
    }
    if(action == "play game")
    {
        Client client;

        if(!client.Connected())
        {
            if(!client.Connect(config.GetValue("Address"), std::atoi(config.GetValue("Port").c_str())))
                this->OpenPopup("Connection error");
        }

        if(client.Connected())
        {
            show_login = true;
            this->SetFocus(1);
        }
    }
    if(action == "credits")
    {
        this->SetState(State::Credits);
    }

    if(show_login)
    {
        this->Login(&show_login);
    }

    ImGui::End();
}

void GUI::CreateAccount()
{
    static char acc_name[16], password[12], password_again[12], real_name[64], location[64], email[64];
    Config config;
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowPos(ImVec2(220.0f, 200.0f), ImGuiCond_FirstUseEver);
    ImGui::Begin("Create account", 0, flags);

    if(this->input_focus == 1)
    {
        ImGui::SetKeyboardFocusHere();
        this->input_focus = 0;
    }
    ImGui::InputText("Account name", acc_name, IM_ARRAYSIZE(acc_name));
    ImGui::InputText("Password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);
    ImGui::InputText("Password again", password_again, IM_ARRAYSIZE(password_again), ImGuiInputTextFlags_Password);
    ImGui::InputText("Real name", real_name, IM_ARRAYSIZE(real_name));
    ImGui::InputText("Location", location, IM_ARRAYSIZE(location));
    bool do_create = false;
    if(ImGui::InputText("Email address", email, IM_ARRAYSIZE(email), ImGuiInputTextFlags_EnterReturnsTrue))
    {
        do_create = true;
    }

    if(ImGui::Button("Create"))
    {
        do_create = true;
    }
    if(ImGui::Button("Cancel"))
    {
        this->SetState(State::MainMenu);
    }

    if(do_create)
    {
        Client client;

        if(!client.Connected())
        {
            if(!client.Connect(config.GetValue("Address"), std::atoi(config.GetValue("Port").c_str())))
                this->OpenPopup("Connection error");
        }

        if(client.Connected())
        {
            std::cout << "CLICK" << std::endl;
            std::array<std::string, 5> input_data;

            input_data[0] = acc_name;
            input_data[1] = password;
            input_data[2] = real_name;
            input_data[3] = location;
            input_data[4] = email;

            client.CreateAccount(input_data);
        }
    }

    ImGui::End();
}

void GUI::Login(bool *p_open)
{
    static char username[32];
    static char password[16];
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowPos(ImVec2(220.0f, 200.0f), ImGuiCond_FirstUseEver);
    ImGui::Begin("Login", p_open, flags);

    if(this->input_focus == 1)
    {
        ImGui::SetKeyboardFocusHere();
        this->input_focus = 0;
    }
    ImGui::InputText("Username", username, IM_ARRAYSIZE(username));
    ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_Password | ImGuiInputTextFlags_EnterReturnsTrue;
    bool do_login = false;
    if(ImGui::InputText("Password", password, IM_ARRAYSIZE(password), input_flags))
    {
        do_login = true;
    }
    if(ImGui::Button("Login"))
    {
        do_login = true;
    }

    if(do_login)
    {
        std::string usr = username;
        std::string pass = password;
        Client client;

        if(!client.Connected())
        {
            if(!client.Connect(Config().GetValue("Address"), 8078))
            {
                this->OpenPopup("Connection error");
            }
        }

        if(client.Connected())
        {
            if(!usr.empty() && !pass.empty()) Client().Login(usr, pass);
        }
    }

    ImGui::End();
}

void GUI::Credits()
{
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowPos(ImVec2(220.0f, 200.0f), ImGuiCond_FirstUseEver);
    ImGui::Begin("Credits", 0, flags);

    ImGui::Text("---------");
    ImGui::Text("Code:");
    ImGui::Text("Jimmyee");
    ImGui::Text("---------");

    if(ImGui::Button("OK"))
    {
        this->SetState(State::MainMenu);
    }

    ImGui::End();
}

void GUI::CharacterList()
{
    Client client;
    GFXLoader gfx_loader;
    Map map;
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Character list", NULL, flags);

    ALLEGRO_BITMAP *bitmap = NULL;
    bitmap = gfx_loader.GetBitmap(8, 1);
    int width = al_get_bitmap_width(bitmap);
    int height = al_get_bitmap_height(bitmap);

    static std::string name = "";
    for(std::size_t i = 0; i < client.characters.size(); ++i)
    {
        ImGui::Text(client.characters[i]->name.c_str());

        int w = al_get_bitmap_width(bitmap);
        int frame_w = w / 4;
        int gender_offset = (int)client.characters[i]->gender * frame_w;

        ImVec2 uv0 = ImVec2(gender_offset, 0);
        ImVec2 uv1 = ImVec2(0.25, 1);
        ImGui::Image(bitmap, ImVec2(width / 4, height), uv0, uv1, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));

        ImGui::PushID(i);
        if(ImGui::Button("Login"))
        {
            client.SelectCharacter(client.characters[i]->name);
        }
        ImGui::PopID();
        ImGui::SameLine();
        bool delete_char = false;
        ImGui::PushID(i);
        if(ImGui::Button("Delete"))
        {
            delete_char = true;
        }
        ImGui::PopID();

        if(delete_char)
        {
            ImGui::OpenPopup("Delete character");
            name = client.characters[i]->name;
        }


    }

    flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
    if(ImGui::BeginPopupModal("Delete character", NULL, flags))
    {
        ImGui::Text("Are you sure?");
        if(ImGui::Button("Yes"))
        {
            client.DeleteCharacter(name);
            ImGui::CloseCurrentPopup();
        }
        if(ImGui::Button("No"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    if(ImGui::Button("Create"))
    {
        this->SetState(State::CreateCharacter);
    }

    ImGui::SameLine();
    if(ImGui::Button("Logout"))
    {
        Client().Disconnect();
        this->SetState(State::MainMenu);
    }

    ImGui::End();
}

void GUI::CreateCharacter()
{
    Client client;
    static char name[12] = "";
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Create character", 0, flags);

    ImGui::InputText("Name", name, IM_ARRAYSIZE(name));
    ImGui::Text("Gender:");
    static int i = 0;
    ImGui::RadioButton("Female", &i, 0); ImGui::SameLine();
    ImGui::RadioButton("Male", &i, 1);

    if(ImGui::Button("Create"))
    {
        client.CreateCharacter(name, (Gender)i);
    }
    if(ImGui::Button("Cancel"))
    {
        this->SetState(State::CharacterList);
    }

    ImGui::End();
}

void GUI::Playing()
{
    Client client;
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

    ImGui::SetNextWindowPos(ImVec2(10, 308));
    ImGui::SetNextWindowSize(ImVec2(620, 162));
    ImGui::Begin("Game Panel", 0, flags);

    this->ChatBox();

    ImGui::End();
}

void GUI::ChatBox()
{
    Chat chat;
    static char chat_input[255] = "";

    if(this->input_focus == 1)
    {
        ImGui::SetKeyboardFocusHere();
        this->input_focus = 0;
    }

    ImGui::SetCursorPos(ImVec2(116, 0));
    ImGui::PushItemWidth(454);
    ImGuiInputTextFlags it_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AlwaysInsertMode;
    std::string message = chat_input;
    if(ImGui::InputText("", chat_input, IM_ARRAYSIZE(chat_input), it_flags))
    {
        if(!message.empty())
        {
            Client().character->Talk(0, message);
            Client().Talk(0, message);
            strcpy(chat_input, "");
        }
        this->SetFocus(1);
    }
    ImGui::PopItemWidth();

    ImGui::SetCursorPos(ImVec2(92, 22));
    ImGui::BeginChildFrame(0, ImVec2(484, 118), 0);

    ImGui::PushTextWrapPos(470);
    for(auto &it : chat.messages)
    {
        std::string name = std::get<0>(it);
        name[0] = std::toupper(name[0]);
        name += ": ";
        ImGui::Text(name.c_str()); ImGui::SameLine();
        ImGui::Text(std::get<1>(it).c_str());

    }
    ImGui::PopTextWrapPos();
    if(chat.new_message)
    {
        ImGui::SetScrollPosHere();
        chat.new_message = false;
    }

    ImGui::EndChildFrame();
}

Chat::Chat()
{
    if(this->initialized_)
    {
        this->new_message = false;

        this->initialized_ = false;
    }
}

void Chat::AddMessage(std::string char_name, std::string message)
{
    if(this->messages.size() >= 255) this->messages.erase(this->messages.begin());

    this->messages.push_back(std::make_pair(char_name, message));
    this->new_message = true;
}
