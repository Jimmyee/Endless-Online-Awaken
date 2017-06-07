#include "gui.hpp"
#include "singleton.hpp"
#include "eoclient.hpp"

#include <imgui-SFML.h>
#include <imgui.h>
#include <chrono>
#include <random>

GUI::GUI(sf::RenderWindow& window_)
: window(window_)
{
    this->state = State::StartScreen;
    this->child_window = 0;
    this->connection_closed = false;

    ImGui::SFML::Init(window, false);

    ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.0f);
    ImGui::GetStyle().WindowPadding = ImVec2(0, 0);
    ImGui::GetStyle().FramePadding = ImVec2(1, 1);

    ImGui::GetIO().Fonts->AddFontFromFileTTF("./font/micross.ttf", 10.f);
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
    S &s = S::GetInstance();

    if(this->state == State::StartScreen || this->state == State::PlayGame
       || this->state == State::Login || this->state == State::Credits)
    {
        this->StartScreen();
    }

    if(this->state == State::StartScreen)
    {

    }
    else if(this->state == State::CreateAccount)
    {

    }
    else if(this->state == State::PlayGame)
    {
        if(s.eoclient->GetState() == EOClient::Initialized)
        {
            this->state = State::Login;
        }
    }
    else if(this->state == State::Credits)
    {

    }
    else if(this->state == State::Login)
    {
        this->LoginBox();
    }

    if(this->connection_closed)
    {
        // display popup with information and after it's closed:
        /*this->Reset();
        this->connection_closed = false;*/
    }
}

void GUI::Draw()
{
    S &s = S::GetInstance();

    EOClient::ClientState state = s.eoclient->GetState();
    if(this->bg.get() && (state == EOClient::Uninitialized || state == EOClient::Initialized || state == EOClient::LoggedIn))
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

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoScrollbar;

    ImGui::SetNextWindowSize(ImVec2(640,480));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGui::Begin("main_window", new bool(true), window_flags);

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

    switch(pressed_id)
    {
    case 0:
        // create account
        break;

    case 1:
        // play game
        if(!s.eoclient->Connected())
        {
            this->state = State::PlayGame;
            if(s.eoclient->Connect())
            {
                s.eoclient->RequestInit();
            }
        }
        else if(s.eoclient->Connected() && s.eoclient->GetState() == EOClient::Initialized)
        {
            this->state = State::Login;
        }
        break;

    case 2:
        // view credits
        break;

    case 3:
        // exit
        s.call_exit = true;
        break;

    default:
        break;
    }

    ImGui::End();
}

void GUI::LoginBox()
{
    S &s = S::GetInstance();

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoScrollbar;

    shared_ptr<sf::Texture> tex = s.gfx_loader->LoadTexture(1, 2, false);

    ImGui::SetNextWindowSize(tex->getSize());
    ImGui::SetNextWindowPos(ImVec2(264, 278));

    ImGui::Begin("login_box", new bool(true), window_flags);
    ImGui::SetCursorPos(ImVec2(0, 0));
    ImGui::Image(*tex);
    // TODO: buttons and edit boxes
    ImGui::End();
}
