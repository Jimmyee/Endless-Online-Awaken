#include "gui.hpp"
#include "singleton.hpp"
#include "eoclient.hpp"
#include "character.hpp"

#include <imgui-SFML.h>
#include <imgui.h>
#include <chrono>
#include <random>

/*

TODO: character list
TODO: display popup with information when connection is lost

*/

GUI::CreateAccount::CreateAccount(char *username, char *password, char *real_name, char *location, char *email)
{
    this->username = username;
    this->password = password;
    this->real_name = real_name;
    this->location = location;
    this->email = email;
    this->approved = false;
    this->created = false;
}

GUI::PopupModal::PopupModal(const char *str_id, std::string title, std::string message, int type)
{
    this->str_id = str_id;
    this->title = title;
    this->message = message;
    this->type = type;
}

GUI::GUI(sf::RenderWindow& window_)
: window(window_)
{
    this->state = State::StartScreen;
    this->connection_closed = false;

    ImGui::SFML::Init(window, false);

    ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.0f);
    ImGui::GetStyle().WindowPadding = ImVec2(0, 0);
    ImGui::GetStyle().FramePadding = ImVec2(1, 1);

    ImGui::GetIO().Fonts->AddFontFromFileTTF("./font/micross.ttf", 10.f);
    ImGui::GetIO().Fonts->AddFontFromFileTTF("./font/micross.ttf", 16.f);
    ImGui::SFML::UpdateFontTexture();

    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, ImVec4(239, 222, 189, 0.2));
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, ImVec4(239, 222, 189, 0.4));
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, ImVec4(239, 222, 189, 0.7));

    S &s = S::GetInstance();
    this->version_address = "0.0.1 - " + s.config->values["Address"];
    std::transform(version_address.begin(), version_address.end(), version_address.begin(), ::toupper);
}

void GUI::ProcessEvent(sf::Event &event)
{
    ImGui::SFML::ProcessEvent(event);
}

void GUI::Update()
{
    ImGui::SFML::Update(this->window, this->clock.restart());
}

void GUI::Process()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoScrollbar;

    ImGui::SetNextWindowSize(ImVec2(640,480));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGui::Begin("main_window", NULL, window_flags);

    if(this->state == State::StartScreen || this->state == State::RequestCreateAccount
       || this->state == State::RequestPlayGame || this->state == State::Login || this->state == State::Credits)
    {
        this->StartScreen();
    }
    else if(this->state == State::CreateAccount)
    {
        this->AccountCreation();
    }
    else if(this->state == State::CharacterList)
    {
        this->CharacterList();
    }
    else if(this->state == State::PlayGame)
    {
        this->GameWindow();
    }

    if(this->popup_modal.get())
    {
        this->DisplayPopupModal();
    }

    ImGui::End();

    if(this->connection_closed)
    {
        this->Reset();
        this->connection_closed = false;
    }
}

void GUI::Draw()
{
    if(this->bg.get())
    {
        this->window.draw(*this->bg.get());
    }

    ImGui::SFML::Render(this->window);
}

void GUI::Shutdown()
{
    ImGui::SFML::Shutdown();
}

void GUI::Reset()
{
    this->state = State::StartScreen;
}

void GUI::SetState(GUI::State state)
{
    this->state = state;
    if(this->state == State::PlayGame)
    {
        this->bg.reset();
    }
}

GUI::State GUI::GetState()
{
    return this->state;
}

void GUI::Disconnected()
{
    this->connection_closed = true;
    puts("DISCONNECTED");
}

void GUI::StartScreen()
{
    S &s = S::GetInstance();

    if(!this->bg.get())
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> uniform_dist(0, 6);
        int rand_num = uniform_dist(gen);

        int tex_id = 30 + rand_num;
        shared_ptr<sf::Texture> texture = s.gfx_loader->LoadTexture(1, tex_id);
        this->bg = shared_ptr<sf::Sprite>(new sf::Sprite(*texture.get()));
    }

    ImGui::SetCursorPos(ImVec2(20, 452));
    ImGui::Text(this->version_address.c_str());

    shared_ptr<sf::Texture> tex = s.gfx_loader->LoadTexture(1, 13);

    int cx = 24;
    int cy = 270;
    int pressed_id = -1;

    for(int button_id = 0; button_id < 4; button_id++)
    {
        sf::Sprite btn1(*tex.get(), sf::IntRect(0, button_id * 40, 180, 40));
        sf::Sprite btn2(*tex.get(), sf::IntRect(180, button_id * 40, 180, 40));

        ImGui::SetCursorPos(ImVec2(cx, cy));
        btn1.setTextureRect(sf::IntRect(0, button_id * 40, 180, 40));
        btn2.setTextureRect(sf::IntRect(180, button_id * 40, 180, 40));

        ImGui::PushID(button_id);
        bool pressed = ImGui::ImageAnimButton(btn1, btn2);
        ImGui::PopID();
        cy += 41 + ImGui::GetStyle().FramePadding.y;

        if(pressed)
        {
            pressed_id = button_id;
        }
    }

    // pressed_id
    // 0 = create account, 1 = play game
    // 2 = view credits, 3 = exit game

    if(pressed_id == 0 || pressed_id == 1)
    {
        if(!s.eoclient->Connected())
        {
            this->state = pressed_id == 0? State::RequestCreateAccount : State::RequestPlayGame;
            if(s.eoclient->Connect())
            {
                s.eoclient->RequestInit();
            }
        }
        else if(s.eoclient->Connected() && s.eoclient->GetState() == EOClient::State::Initialized)
        {
            this->state = pressed_id == 0? State::CreateAccount : State::Login;
            this->initialize_focus = true;
        }
    }
    else if(pressed_id == 2)
    {
        // credits
    }
    else if(pressed_id == 3)
    {
        s.call_exit = true;
    }

    if(this->state == State::RequestPlayGame)
    {
        if(s.eoclient->GetState() == EOClient::State::Initialized)
        {
            this->state = State::Login;
            this->initialize_focus = true;
        }
    }
    else if(this->state == State::RequestCreateAccount)
    {
        if(s.eoclient->GetState() == EOClient::State::Initialized)
        {
            this->state = State::CreateAccount;
            this->initialize_focus = true;
        }
    }

    if(this->state == State::Login)
    {
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoScrollbar;

        shared_ptr<sf::Texture> tex = s.gfx_loader->LoadTexture(1, 2, false);

        ImGui::SetNextWindowSize(tex->getSize());
        ImGui::SameLine();
        ImGui::SetNextWindowPos(ImVec2(264, 278));
        ImGui::SetCursorPos(ImVec2(264, 278));

        ImGui::BeginChild("login_box", ImVec2(tex->getSize()), NULL, window_flags);
        ImGui::SetCursorPos(ImVec2(0, 0));
        ImGui::Image(*tex);

        ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImColor(165, 130, 105, 255));
        ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImColor(0, 0, 0, 255));
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

        static char buf_username[32] = "";
        static char buf_password[32] = "";
        ImGui::SameLine();
        if(this->initialize_focus)
        {
            ImGui::SetKeyboardFocusHere();
            this->initialize_focus = false;
        }
        ImGui::PushItemWidth(140);
        ImGui::SetCursorPos(ImVec2(136, 33));
        ImGui::PushID(1);
        ImGui::InputText("", buf_username, sizeof(buf_username));
        ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0,0,0,255));
        ImGui::PopID();

        ImGui::SetCursorPos(ImVec2(136, 68));
        ImGui::PushID(2);
        if(ImGui::InputText("", buf_password, sizeof(buf_password), ImGuiInputTextFlags_Password | ImGuiInputTextFlags_EnterReturnsTrue))
        {
            std::string username(buf_username);
            std::string password(buf_password);

            if(username.length() >= 4 && password.length() >= 4)
            {
                s.eoclient->LoginRequest(username, password);
            }
        }
        ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0,0,0,255));
        ImGui::PopID();

        ImGui::PopItemWidth();
        ImGui::PopStyleColor(2);
        ImGui::PopFont();

        tex = s.gfx_loader->LoadTexture(1, 15);
        sf::Sprite btn1(*tex, sf::IntRect(0, 0, 91, 29));
        sf::Sprite btn2(*tex, sf::IntRect(91, 0, 91, 29));
        ImGui::SetCursorPos(ImVec2(94, 104));
        ImGui::PushID(3);
        if(ImGui::ImageAnimButton(btn1, btn2))
        {
            std::string username(buf_username);
            std::string password(buf_password);

            if(username.length() >= 4 && password.length() >= 4)
            {
                s.eoclient->LoginRequest(username, password);
            }
        }
        ImGui::PopID();

        btn1.setTextureRect(sf::IntRect(0, 29, 91, 29));
        btn2.setTextureRect(sf::IntRect(91, 29, 91, 29));
        ImGui::SetCursorPos(ImVec2(186 + ImGui::GetStyle().FramePadding.x, 104));
        ImGui::PushID(4);
        if(ImGui::ImageAnimButton(btn1, btn2))
        {
            this->state = State::StartScreen;
        }
        ImGui::PopID();

        ImGui::EndChild();
    }
}

void GUI::DisplayPopupModal()
{
    S &s = S::GetInstance();

    if(!this->popup_modal.get()) return;

    shared_ptr<sf::Texture> tex = s.gfx_loader->LoadTexture(1, 18);

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoScrollbar;

    ImGui::SetNextWindowSize(tex->getSize());
    if(ImGui::BeginPopupModal(this->popup_modal->str_id, NULL, window_flags))
    {
        ImGui::SetCursorPos(ImVec2(0, 0));
        ImGui::Image(*tex);
        ImGui::SameLine();
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
        ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImColor(240, 240, 200, 255));
        ImGui::SetCursorPos(ImVec2(60, 29));
        ImGui::Text(this->popup_modal->title.c_str());
        ImGui::SetCursorPos(ImVec2(21, 64));
        ImGui::TextWrapped(this->popup_modal->message.c_str());
        ImGui::PopFont();
        ImGui::PopStyleColor();

        tex = s.gfx_loader->LoadTexture(1, 15);
        ImGui::SetCursorPos(ImVec2(180, 112));
        if(ImGui::ImageAnimButton(sf::Sprite(*tex, sf::IntRect(0, 4 * 29, 91, 29)), sf::Sprite(*tex, sf::IntRect(91, 4 * 29, 91, 29))))
        {
            this->SetState(State::CreateAccount);
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void GUI::AccountCreation()
{
    S &s = S::GetInstance();

    static char username[32] = "";
    static char password[32] = "";
    static char password_again[32] = "";
    static char real_name[32] = "";
    static char location[32] = "";
    static char email[32] = "";

    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImColor(165, 130, 105, 255));
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImColor(0, 0, 0, 255));
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

    shared_ptr<sf::Texture> tex = s.gfx_loader->LoadTexture(1, 12);

    ImGui::PushItemWidth(242);

    ImGui::SetCursorPos(ImVec2(350, 51));
    ImGui::Image(*tex, sf::FloatRect(0, 0, 149, 15));

    if(this->initialize_focus)
    {
        ImGui::SetKeyboardFocusHere();
        this->initialize_focus = false;
    }

    ImGui::SetCursorPos(ImVec2(350, 69));
    ImGui::PushID(1);
    ImGui::InputText("", username, sizeof(username));
    ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0,0,0,255));
    ImGui::PopID();


    ImGui::SetCursorPos(ImVec2(350, 102));
    ImGui::Image(*tex, sf::FloatRect(0, 15, 149, 14));

    ImGui::SetCursorPos(ImVec2(350, 120));
    ImGui::PushID(2);
    ImGui::InputText("", password, sizeof(password), ImGuiInputTextFlags_Password);
    ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0,0,0,255));
    ImGui::PopID();

    ImGui::SetCursorPos(ImVec2(350, 154));
    ImGui::Image(*tex, sf::FloatRect(0, 30, 149, 14));

    ImGui::SetCursorPos(ImVec2(350, 171));
    ImGui::PushID(3);
    ImGui::InputText("", password_again, sizeof(password_again), ImGuiInputTextFlags_Password);
    ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0,0,0,255));
    ImGui::PopID();

    ImGui::SetCursorPos(ImVec2(350, 242));
    ImGui::Image(*tex, sf::FloatRect(0, 45, 149, 14));

    ImGui::SetCursorPos(ImVec2(350, 260));
    ImGui::PushID(4);
    ImGui::InputText("", real_name, sizeof(real_name));
    ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0,0,0,255));
    ImGui::PopID();

    ImGui::SetCursorPos(ImVec2(350, 293));
    ImGui::Image(*tex, sf::FloatRect(0, 60, 149, 14));

    ImGui::SetCursorPos(ImVec2(350, 311));
    ImGui::PushID(5);
    ImGui::InputText("", location, sizeof(location));
    ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0,0,0,255));
    ImGui::PopID();
    ImGui::PopItemWidth();

    ImGui::SetCursorPos(ImVec2(350, 344));
    ImGui::Image(*tex, sf::FloatRect(0, 75, 149, 14));

    ImGui::PushItemWidth(221);
    ImGui::SetCursorPos(ImVec2(350, 362));
    ImGui::PushID(6);
    ImGui::InputText("", email, sizeof(email));
    ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0,0,0,255));
    ImGui::PopID();
    ImGui::PopItemWidth();

    sf::Sprite btn1(*tex, sf::IntRect(3, 93, 20, 20));
    sf::Sprite btn2(*tex, sf::IntRect(22, 93, 20, 20));

    ImGui::SetCursorPos(ImVec2(571, 360));
    ImGui::PushID(7);
    if(ImGui::ImageAnimButton(btn1, btn2))
    {
        std::string stremail(email);

        if(stremail.length() < 32)
        {
            strcat(email, "@");
        }
    }
    ImGui::PopID();

    tex = s.gfx_loader->LoadTexture(1, 14);
    btn1.setTexture(*tex);
    btn1.setTextureRect(sf::IntRect(0, 0, 120, 40));
    btn2.setTexture(*tex);
    btn2.setTextureRect(sf::IntRect(120, 0, 120, 40));

    ImGui::SetCursorPos(ImVec2(350, 416));
    ImGui::PushID(8);

    if(ImGui::ImageAnimButton(btn1, btn2))
    {
        this->create_account = shared_ptr<CreateAccount>(new CreateAccount(username, password, real_name, location, email));
        s.eoclient->AccountRequest(this->create_account->username);
    }
    ImGui::PopID();

    btn1.setTextureRect(sf::IntRect(0, 40, 120, 40));
    btn2.setTextureRect(sf::IntRect(120, 40, 120, 40));

    // button: Cancel
    ImGui::SetCursorPos(ImVec2(472, 416));
    ImGui::PushID(9);
    if(ImGui::ImageAnimButton(btn1, btn2))
    {
        this->state = State::StartScreen;
    }
    ImGui::PopID();

    ImGui::PopStyleColor(2);
    ImGui::PopFont();

    if(this->create_account.get())
    {
        if(this->create_account->approved && this->create_account->creation_clock.getElapsedTime().asSeconds() >= 1.0)
        {
            std::vector<std::string> acc_info = this->GetAccountInfo();
            s.eoclient->AccountCreate(acc_info[0], acc_info[1], acc_info[2], acc_info[3], acc_info[4]);
            this->create_account.reset();
        }

        ImGui::OpenPopup("createacc");

        shared_ptr<sf::Texture> tex = s.gfx_loader->LoadTexture(1, 23);

        ImGui::SetNextWindowSize(tex->getSize());

        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoScrollbar;

        if(ImGui::BeginPopupModal("createacc", NULL, window_flags))
        {
            ImGui::SetCursorPos(ImVec2(0, 0));
            ImGui::Image(*tex);
            ImGui::SameLine();
            ImGui::SetCursorPos(ImVec2(18, 11));
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
            ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImColor(240, 240, 200, 255));
            ImGui::Text("Creating account...");
            ImGui::PopFont();
            ImGui::PopStyleColor();

            tex = s.gfx_loader->LoadTexture(1, 15);
            ImGui::SetCursorPos(ImVec2(180, 80));
            if(ImGui::ImageAnimButton(sf::Sprite(*tex, sf::IntRect(0, 29, 91, 29)), sf::Sprite(*tex, sf::IntRect(91, 29, 91, 29))))
            {
                this->SetState(State::CreateAccount);
                this->create_account.reset();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }
}

std::vector<std::string> GUI::GetAccountInfo()
{
    std::vector<std::string> ret;

    ret.push_back(this->create_account->username);
    ret.push_back(this->create_account->password);
    ret.push_back(this->create_account->real_name);
    ret.push_back(this->create_account->location);
    ret.push_back(this->create_account->email);

    return ret;
}

void GUI::ResetCreateAcc()
{
    this->create_account.reset();
}

void GUI::CharacterList()
{
    S &s = S::GetInstance();

    shared_ptr<sf::Texture> tex1 = s.gfx_loader->LoadTexture(1, 11, false);
    int pressed_id = -1;
    bool pressed_login = false;
    bool pressed_delete = false;
    int button_id = 0;
    for(std::size_t i = 0; i < 3; ++i)
    {
        int y = 36 + i * tex1->getSize().y + i;
        ImGui::SetCursorPos(ImVec2(325, y));
        ImGui::PushID(i);
        ImGui::Image(*tex1);
        ImGui::PopID();
        shared_ptr<sf::Texture> tex2 = s.gfx_loader->LoadTexture(1, 15);
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(325 + 166, y + 27));
        shared_ptr<Character> character = s.eoclient->GetAccountCharacter(i);
        if(character.get())
        {
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
            ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImColor(180, 160, 140, 255));
            ImGui::Text(character->name.c_str());
            ImGui::PopFont();
            ImGui::PopStyleColor();
        }
        ImGui::SetCursorPos(ImVec2(325 + 160, y + 55));
        ImGui::PushID(button_id++);
        if(ImGui::ImageAnimButton(sf::Sprite(*tex2, sf::IntRect(0, 2 * 29, 91, 29)),
                                               sf::Sprite(*tex2, sf::IntRect(91, 2 * 29, 91, 29))))
        {
            pressed_login = true;
        }
        ImGui::PopID();
        ImGui::SetCursorPos(ImVec2(325 + 160, y + 84 + 1 + ImGui::GetStyle().FramePadding.y));
        ImGui::PushID(button_id++);
        if(ImGui::ImageAnimButton(sf::Sprite(*tex2, sf::IntRect(0, 3 *29, 91, 29)),
                                                sf::Sprite(*tex2, sf::IntRect(91, 3 *29, 91, 29))))
        {
             pressed_delete = true;
        }
        ImGui::PopID();

        if(pressed_login || pressed_delete)
        {
            pressed_id = i;
            break;
        }
    }

    if(pressed_login)
    {
        shared_ptr<Character> character = s.eoclient->GetAccountCharacter(pressed_id);

        if(character.get())
        {
            s.eoclient->RequestSelectCharacter(character->id);
        }
        else
        {
            // player selects empty character slot
        }
    }
    else if(pressed_delete)
    {

    }

    tex1 = s.gfx_loader->LoadTexture(1, 14);
    ImGui::SetCursorPos(ImVec2(324, 416));
    if(ImGui::ImageAnimButton(sf::Sprite(*tex1, sf::IntRect(0, 0, 120, 40)),
                                               sf::Sprite(*tex1, sf::IntRect(120, 0, 120, 40))))
    {
       // create character button pressed
    }

    ImGui::SetCursorPos(ImVec2(324 + 120 + 1 + ImGui::GetStyle().FramePadding.y, 416));
    if(ImGui::ImageAnimButton(sf::Sprite(*tex1, sf::IntRect(0, 3 * 40, 120, 40)),
                                               sf::Sprite(*tex1, sf::IntRect(120, 3 * 40, 120, 40))))
    {
       // change password button pressed
    }
}

void GUI::GameWindow()
{
    // main game screen!
}
