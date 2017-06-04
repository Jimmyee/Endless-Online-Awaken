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
    ImGui::SFML::Init(window, false);

    ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.0f);
    ImGui::GetStyle().WindowPadding = ImVec2(0, 0);
    ImGui::GetIO().Fonts->AddFontFromFileTTF("./font/micross.ttf", 10.f);
    ImGui::SFML::UpdateFontTexture();

    S &s = S::GetInstance();
    this->version_address = "0.0.1 - " + s.config->values["Address"];
    std::transform(version_address.begin(), version_address.end(), version_address.begin(), ::toupper);
}

GUI::~GUI()
{
    ImGui::SFML::Shutdown();
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

    EOClient::ClientState state = s.eoclient->GetState();

    if(state == EOClient::Uninitialized || state == EOClient::Initialized)
    {
        this->StartScreen();
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

    shared_ptr<sf::Texture> tex = s.gfx_loader->LoadTexture(1, 3);

    ImGui::SetCursorPos(ImVec2(50, 200));
    ImGui::ImageButton(*tex.get(), 1);

    ImGui::End();
}
