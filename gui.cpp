// Endless Online Awaken v0.0.1

#include "gui.hpp"
#include "singleton.hpp"
#include "eoclient.hpp"
#include "character.hpp"

#include <imgui-SFML.h>
#include <imgui.h>
#include <chrono>
#include <random>

/*

*/

struct TextFilters
{
    static int FilterImGuiLetters(ImGuiTextEditCallbackData* data)
    {
        if (data->EventChar == 32 || (data->EventChar >= 48 && data->EventChar <= 57) ||
            (data->EventChar >= 65 && data->EventChar <= 90) || (data->EventChar >= 97 && data->EventChar <= 122))
        {
            return 0;
        }

        return 1;
    }

    static int FilterImGuiLettersEmail(ImGuiTextEditCallbackData* data)
    {
        if (data->EventChar == 32 || (data->EventChar >= 48 && data->EventChar <= 57) ||
            (data->EventChar >= 65 && data->EventChar <= 90) || (data->EventChar >= 97 && data->EventChar <= 122) ||
            data->EventChar == 46 || data->EventChar == 64)
        {
            return 0;
        }

        return 1;
    }
};

GUI::CreateAccount::CreateAccount()
{
    this->approved = false;
    this->created = false;
}

GUI::PopupModal::PopupModal(const char *str_id, std::string title, std::string message, int type)
{
    this->str_id = str_id;
    this->title = title;
    this->message = message;
    this->type = type;
    this->open = false;
    this->need_open = true;
}

GUI::TextField::TextField(std::string text, std::size_t min_len, std::size_t max_len)
{
    this->text = text;
    this->min_len = min_len;
    this->max_len = max_len;
    this->text.resize(max_len);
}

std::string GUI::TextField::GetText()
{
    return std::string(this->text.c_str());
}

bool GUI::TextField::ValidateLength()
{
    std::string text_clean = this->GetText();

    if(!(text_clean.length() >= this->min_len && text_clean.length() <= this->max_len)) return false;

    return true;
}

GUI::ChatConsole::ChatMessage::ChatMessage(std::string name, std::string message)
{
    this->name = name;
    this->message = message;
}

GUI::ChatConsole::ChatConsole()
{
    this->last_buffer_size = 0;
}

void GUI::ChatConsole::AddMessage(ChatMessage message)
{
    message.name[0] = std::toupper(message.name[0]);
    this->buffer.push_back(message);

    if(this->buffer.size() > 255)
    {
        this->buffer.erase(this->buffer.begin());
    }
}

void GUI::ChatConsole::Draw()
{
    S &s = S::GetInstance();
    shared_ptr<sf::Texture> tex = s.gfx_loader->LoadTexture(2, 28);

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_HorizontalScrollbar;

    ImGui::SetNextWindowSize(tex->getSize());
    ImGui::SetNextWindowContentSize(ImVec2(460.0f, 98.0f));

    ImGui::BeginChild("chat", tex->getSize(), NULL, window_flags);

    ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
    ImGui::Image(*tex);
    ImGui::SameLine();
    ImGui::SetNextWindowSize(tex->getSize());
    ImGui::SetCursorPos(ImVec2(22.0f, 2.0f));
    ImGui::BeginChild("chat_buffer", ImVec2(460.0f, 96.0f), NULL, window_flags);
    ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4,1)); // Tighten spacing
    for(std::size_t i = 0; i < this->buffer.size(); ++i)
    {
        std::string str = this->buffer[i].name + ": " + this->buffer[i].message;
        ImGui::TextWrapped(str.c_str());
    }
    ImGui::PopStyleVar();
    if(this->last_buffer_size < this->buffer.size())
    {
        ImGui::SetScrollHere();
        this->last_buffer_size = this->buffer.size();
    }
    ImGui::EndChild();

    ImGui::EndChild();
}

GUI::GUI(sf::RenderWindow& window_)
: window(window_)
{
    ImGui::SFML::Init(window, false);

    ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    ImGui::GetStyle().WindowPadding = ImVec2(0.0f, 0.0f);
    ImGui::GetStyle().FramePadding = ImVec2(1.0f, 1.0f);

    ImGui::GetIO().Fonts->AddFontFromFileTTF("./font/micross.ttf", 10.0f);
    ImGui::GetIO().Fonts->AddFontFromFileTTF("./font/micross.ttf", 16.0f);
    ImGui::GetIO().Fonts->AddFontFromFileTTF("./font/micross.ttf", 14.0f);
    ImGui::SFML::UpdateFontTexture();
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, ImVec4(239.0f, 222.0f, 189.0f, 0.2f));
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, ImVec4(239.0f, 222.0f, 189.0f, 0.4f));
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, ImVec4(239.0f, 222.0f, 189.0f, 0.7f));

    S &s = S::GetInstance();
    this->version_address = "0.0.1 - " + s.config->values["Address"];
    std::transform(version_address.begin(), version_address.end(), version_address.begin(), ::toupper);

    this->SetState(State::StartScreen);
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
    if(this->state == State::StartScreen)
    {

    }
    else if(this->state == State::CreateAccount)
    {

    }
    else if(this->state == State::CharacterList)
    {

    }
    else if(this->state == State::PlayGame)
    {

    }
}

void GUI::Draw()
{
    if(this->bg.get())
    {
        this->window.draw(*this->bg);
    }

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoScrollbar;

    ImGui::SetNextWindowSize(ImVec2(640.0f,480.0f));
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));

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
        if(this->popup_modal->need_open)
        {
            this->popup_modal->need_open = false;
            this->popup_modal->open = true;
            ImGui::OpenPopup(this->popup_modal->str_id);
        }

        this->DisplayPopupModal();

        if(!this->popup_modal->open) // popup has been closed
        {
            std::string id = this->popup_modal->str_id;
            if(id == "msg_disconnected")
            {
                this->popup_modal.reset();
                this->Reset();
            }
        }
    }

    ImGui::End();

    ImGui::SFML::Render(this->window);
}

void GUI::Shutdown()
{
    ImGui::SFML::Shutdown();
}

void GUI::Reset()
{
    this->SetState(State::StartScreen);
}

void GUI::SetState(GUI::State state)
{
    State prev_state = this->state;
    this->state = state;
    if(this->state == State::PlayGame)
    {
        this->bg.reset();
    }
    if((this->state == State::StartScreen && prev_state != State::Login) || this->state == State::CreateAccount || this->state == State::CharacterList)
    {
        this->bg_avatar.reset();
    }

    this->text_fields.clear();
    if(this->state == State::CreateAccount)
    {
        this->text_fields.push_back(TextField("", 4, 16)); // account name
        this->text_fields.push_back(TextField("", 6, 12)); // password
        this->text_fields.push_back(TextField("", 6, 12)); // password again
        this->text_fields.push_back(TextField("", 2, 64)); // real name
        this->text_fields.push_back(TextField("", 2, 64)); // location
        this->text_fields.push_back(TextField("", 6, 64)); // email
    }
    else if(this->state == State::Login)
    {
        this->text_fields.push_back(TextField("", 4, 16));
        this->text_fields.push_back(TextField("", 6, 12));
    }
    else if(this->state == State::PlayGame)
    {
        this->text_fields.push_back(TextField("", 1, 255));
    }
}

GUI::State GUI::GetState()
{
    return this->state;
}

void GUI::Disconnected()
{
    std::string title =  "Connection lost";
    std::string message = "Connection with the game server has been lost.";
    this->popup_modal = shared_ptr<PopupModal>(new PopupModal("msg_disconnected", title, message, 0));
}

void GUI::DisplayPopupModal()
{
    S &s = S::GetInstance();

    if(!this->popup_modal.get()) return;

    int tex_ids[3] = { 18, 23, 25 };
    shared_ptr<sf::Texture> tex = s.gfx_loader->LoadTexture(1, tex_ids[this->popup_modal->type]);

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoScrollbar;

    ImGui::SetNextWindowSize(tex->getSize());
    ImVec2 window_c_size[3] = { ImVec2(268.0f, 110.0f), ImVec2(268.0f, 110.0f), ImVec2(268.0f, 110.0f) };
    ImGui::SetNextWindowContentSize(window_c_size[this->popup_modal->type]);
    if(ImGui::BeginPopupModal(this->popup_modal->str_id, NULL, window_flags))
    {
        ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
        ImGui::Image(*tex);
        ImGui::SameLine();
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
        ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImColor(240, 240, 200, 255));
        ImVec2 title_pos[3] = { ImVec2(60.0f, 22.0f), ImVec2(18.0f, 11.0f), ImVec2(60.0f, 22.0f) };
        ImGui::SetCursorPos(title_pos[this->popup_modal->type]);
        ImGui::Text(this->popup_modal->title.c_str());
        ImVec2 msg_pos[3] = { ImVec2(21.0f, 58.0f), ImVec2(21.0f, 58.0f), ImVec2(21.0f, 58.0f) };
        ImGui::SetCursorPos(msg_pos[this->popup_modal->type]);
        ImGui::TextWrapped(this->popup_modal->message.c_str());
        ImGui::PopFont();
        ImGui::PopStyleColor();

        tex = s.gfx_loader->LoadTexture(1, 15);
        ImVec2 button_pos[3] = { ImVec2(180, 112), ImVec2(180, 80), ImVec2(180, 80) };
        int button_ids[3] = { 4, 1, 1 };
        ImGui::SetCursorPos(button_pos[this->popup_modal->type]);
        if(ImGui::ImageAnimButton(sf::Sprite(*tex, sf::IntRect(0, button_ids[this->popup_modal->type] * 29, 91, 29)),
                                   sf::Sprite(*tex, sf::IntRect(91, button_ids[this->popup_modal->type] * 29, 91, 29))))
        {
            this->popup_modal->open = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void GUI::StartScreen()
{
    S &s = S::GetInstance();

    if(!this->bg.get())
    {
        int rand_num = s.rand_gen.RandInt(0, 6);

        int tex_id = 30 + rand_num;
        shared_ptr<sf::Texture> texture = s.gfx_loader->LoadTexture(1, tex_id);
        this->bg = shared_ptr<sf::Sprite>(new sf::Sprite(*texture.get()));
    }

    if(!this->bg_avatar.get())
    {
        int rand_num = s.rand_gen.RandInt(1, 4);

        int tex_id = 40 + rand_num;
        shared_ptr<sf::Texture> texture = s.gfx_loader->LoadTexture(1, tex_id);
        this->bg_avatar = shared_ptr<sf::Sprite>(new sf::Sprite(*texture));
        sf::FloatRect gb = this->bg_avatar->getGlobalBounds();
        this->bg_avatar->setPosition(640 - gb.width - 10, 480 - gb.height - 10);
    }

    this->window.draw(*this->bg_avatar);

    ImGui::SetCursorPos(ImVec2(20.0f, 452.0f));
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
            this->SetState(pressed_id == 0? State::RequestCreateAccount : State::RequestPlayGame);
            if(s.eoclient->Connect())
            {
                s.eoclient->RequestInit();
            }
            else
            {
                std::string title =  "Could not connect";
                std::string message = "Could not connect to the game server.";
                this->popup_modal = shared_ptr<PopupModal>(new PopupModal("msg_notconnect", title, message, 0));
            }
        }
        else if(s.eoclient->Connected() && s.eoclient->GetState() == EOClient::State::Initialized)
        {
            this->SetState(pressed_id == 0? State::CreateAccount : State::Login);
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
            this->SetState(State::Login);
            this->initialize_focus = true;
        }
    }
    else if(this->state == State::RequestCreateAccount)
    {
        if(s.eoclient->GetState() == EOClient::State::Initialized)
        {
            this->SetState(State::CreateAccount);
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
        ImGui::SetNextWindowPos(ImVec2(264.0f, 278.0f));
        ImGui::SetCursorPos(ImVec2(264.0f, 278.0f));

        ImGui::BeginChild("login_box", ImVec2(tex->getSize()), NULL, window_flags);
        ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
        ImGui::Image(*tex);

        ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImColor(165, 130, 105, 255));
        ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImColor(0, 0, 0, 255));
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

        ImGui::SameLine();
        if(this->initialize_focus)
        {
            ImGui::SetKeyboardFocusHere();
            this->initialize_focus = false;
        }
        ImGui::PushItemWidth(140.0f);
        ImGui::SetCursorPos(ImVec2(136.0f, 33.0f));
        ImGui::PushID(1);

        ImGui::InputText("", (char *)this->text_fields[0].text.c_str(), this->text_fields[0].max_len + 1, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);
        ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0, 0, 0, 255));
        ImGui::PopID();

        bool request_login = false;

        ImGui::SetCursorPos(ImVec2(136.0f, 68.0f));
        ImGui::PushID(2);
        if(ImGui::InputText("", (char *)this->text_fields[1].text.c_str(), this->text_fields[1].max_len + 1, ImGuiInputTextFlags_CallbackCharFilter | ImGuiInputTextFlags_Password | ImGuiInputTextFlags_EnterReturnsTrue, TextFilters::FilterImGuiLetters))
        {
            request_login = true;
        }
        ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0, 0, 0, 255));
        ImGui::PopID();

        ImGui::PopItemWidth();
        ImGui::PopStyleColor(2);
        ImGui::PopFont();

        tex = s.gfx_loader->LoadTexture(1, 15);
        sf::Sprite btn1(*tex, sf::IntRect(0, 0, 91, 29));
        sf::Sprite btn2(*tex, sf::IntRect(91, 0, 91, 29));
        ImGui::SetCursorPos(ImVec2(94.0f, 104.0f));
        ImGui::PushID(3);
        if(ImGui::ImageAnimButton(btn1, btn2))
        {
            request_login = true;
        }
        ImGui::PopID();

        if(request_login)
        {
            bool valid = true;
            int field_index = 0;

            for(std::size_t i = 0; i < 2; ++i)
            {
                if(!this->text_fields[i].ValidateLength())
                {
                    valid = false;
                    field_index = i;
                    break;
                }
            }

            if(valid)
            {
                s.eoclient->LoginRequest(this->text_fields[0].GetText(), this->text_fields[1].GetText());
            }
            else
            {
                std::string title =  "Syntax is not correct";
                std::string message = "";
                std::string field_name_offset[2] = { "Username", "Password" };

                message += std::string(" '" + field_name_offset[field_index] + "'" + " length should be between ");
                message += std::to_string(this->text_fields[field_index].min_len) + "-";
                message += std::to_string(this->text_fields[field_index].max_len) + " letters.";

                s.gui->popup_modal = shared_ptr<GUI::PopupModal>(new GUI::PopupModal("msg_login", title, message, 0));
            }
        }

        btn1.setTextureRect(sf::IntRect(0, 29, 91, 29));
        btn2.setTextureRect(sf::IntRect(91, 29, 91, 29));
        ImGui::SetCursorPos(ImVec2(186.0f + ImGui::GetStyle().FramePadding.x, 104.0f));
        ImGui::PushID(4);
        if(ImGui::ImageAnimButton(btn1, btn2))
        {
            this->SetState(State::StartScreen);
        }
        ImGui::PopID();

        ImGui::EndChild();
    }
}

void GUI::AccountCreation()
{
    S &s = S::GetInstance();

    if(!this->bg_avatar.get())
    {
        int rand_num = s.rand_gen.RandInt(1, 8);

        int tex_id = 60 + rand_num;
        shared_ptr<sf::Texture> texture = s.gfx_loader->LoadTexture(1, tex_id);
        this->bg_avatar = shared_ptr<sf::Sprite>(new sf::Sprite(*texture));
        sf::FloatRect gb = this->bg_avatar->getGlobalBounds();
        this->bg_avatar->setPosition(10, 480 - gb.height - 10);
    }

    this->window.draw(*this->bg_avatar);

    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImColor(165, 130, 105, 255));
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImColor(0, 0, 0, 255));
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

    shared_ptr<sf::Texture> tex = s.gfx_loader->LoadTexture(1, 12);

    ImGui::PushItemWidth(242.0f);

    ImGui::SetCursorPos(ImVec2(350.0f, 51.0f));
    ImGui::Image(*tex, sf::FloatRect(0.0f, 0.0f, 149.0f, 15.0f));

    if(this->initialize_focus)
    {
        ImGui::SetKeyboardFocusHere();
        this->initialize_focus = false;
    }

    ImGui::SetCursorPos(ImVec2(350.0f, 69.0f));
    ImGui::PushID(1);
    ImGui::InputText("", (char *)this->text_fields[0].text.c_str(), this->text_fields[0].max_len + 1, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);
    ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0, 0, 0, 255));
    ImGui::PopID();


    ImGui::SetCursorPos(ImVec2(350.0f, 102.0f));
    ImGui::Image(*tex, sf::FloatRect(0.0f, 15.0f, 149.0f, 14.0f));

    ImGui::SetCursorPos(ImVec2(350.0f, 120.0f));
    ImGui::PushID(2);
    ImGui::InputText("", (char *)this->text_fields[1].text.c_str(), this->text_fields[1].max_len + 1, ImGuiInputTextFlags_CallbackCharFilter | ImGuiInputTextFlags_Password, TextFilters::FilterImGuiLetters);
    ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0, 0, 0, 255));
    ImGui::PopID();

    ImGui::SetCursorPos(ImVec2(350.0f, 154.0f));
    ImGui::Image(*tex, sf::FloatRect(0.0f, 30.0f, 149.0f, 14.0f));

    ImGui::SetCursorPos(ImVec2(350.0f, 171.0f));
    ImGui::PushID(3);
    ImGui::InputText("", (char *)this->text_fields[2].text.c_str(), this->text_fields[2].max_len + 1, ImGuiInputTextFlags_CallbackCharFilter | ImGuiInputTextFlags_Password, TextFilters::FilterImGuiLetters);
    ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0, 0, 0, 255));
    ImGui::PopID();

    ImGui::SetCursorPos(ImVec2(350.0f, 242.0f));
    ImGui::Image(*tex, sf::FloatRect(0.0f, 45.0f, 149.0f, 14.0f));

    ImGui::SetCursorPos(ImVec2(350.0f, 260.0f));
    ImGui::PushID(4);
    ImGui::InputText("", (char *)this->text_fields[3].text.c_str(), this->text_fields[3].max_len + 1, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);
    ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0, 0, 0, 255));
    ImGui::PopID();

    ImGui::SetCursorPos(ImVec2(350.0f, 293.0f));
    ImGui::Image(*tex, sf::FloatRect(0.0f, 60.0f, 149.0f, 14.0f));

    ImGui::SetCursorPos(ImVec2(350.0f, 311.0f));
    ImGui::PushID(5);
    ImGui::InputText("", (char *)this->text_fields[4].text.c_str(), this->text_fields[4].max_len + 1, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);
    ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0, 0, 0, 255));
    ImGui::PopID();
    ImGui::PopItemWidth();

    ImGui::SetCursorPos(ImVec2(350.0f, 344.0f));
    ImGui::Image(*tex, sf::FloatRect(0.0f, 75.0f, 149.0f, 14.0f));

    ImGui::PushItemWidth(221.0f);
    ImGui::SetCursorPos(ImVec2(350.0f, 362.0f));
    ImGui::PushID(6);
    ImGui::InputText("", (char *)this->text_fields[5].text.c_str(), this->text_fields[5].max_len + 1, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLettersEmail);
    ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0, 0, 0, 255));
    ImGui::PopID();
    ImGui::PopItemWidth();

    sf::Sprite btn1(*tex, sf::IntRect(3, 93, 20, 20));
    sf::Sprite btn2(*tex, sf::IntRect(22, 93, 20, 20));

    ImGui::SetCursorPos(ImVec2(571.0f, 360.0f));
    ImGui::PushID(7);
    if(ImGui::ImageAnimButton(btn1, btn2))
    {
        if(this->text_fields[5].GetText().length() < this->text_fields[5].max_len)
        {
            strcat((char *)this->text_fields[5].text.c_str(), "@");
        }
    }
    ImGui::PopID();

    tex = s.gfx_loader->LoadTexture(1, 14);
    btn1.setTexture(*tex);
    btn1.setTextureRect(sf::IntRect(0, 0, 120, 40));
    btn2.setTexture(*tex);
    btn2.setTextureRect(sf::IntRect(120, 0, 120, 40));

    ImGui::SetCursorPos(ImVec2(350.0f, 416.0f));
    ImGui::PushID(8);

    if(ImGui::ImageAnimButton(btn1, btn2))
    {
        bool valid = true;
        int field_index = 0;

        for(std::size_t i = 0; i < 6; ++i)
        {
            if(!this->text_fields[i].ValidateLength())
            {
                valid = false;
                field_index = i;
                break;
            }
        }

        bool passwords_same = true;
        if(this->text_fields[1].GetText() != this->text_fields[2].GetText())
        {
            passwords_same = false;
        }

        if(valid && passwords_same)
        {
            std::vector<TextField> cont = this->text_fields;
            this->create_account = shared_ptr<CreateAccount>(new CreateAccount());
            s.eoclient->AccountRequest(this->text_fields[0].GetText());

            std::string title =  "Creating account...";
            this->popup_modal = shared_ptr<PopupModal>(new PopupModal("msg_create_acc", title, "", 1));
        }
        else if(!valid)
        {
            std::string title =  "Syntax is not correct";
            std::string message = "";
            std::string field_name_offset[6] = { "Username", "Password", "Type password again", "Real name", "Location", "Email" };

            message += std::string(" '" + field_name_offset[field_index] + "'" + " length should be between ");
            message += std::to_string(this->text_fields[field_index].min_len) + "-";
            message += std::to_string(this->text_fields[field_index].max_len) + " letters.";

            s.gui->popup_modal = shared_ptr<GUI::PopupModal>(new GUI::PopupModal("msg_login", title, message, 0));
        }
        else if(!passwords_same)
        {
            std::string title =  "Syntax is not correct";
            std::string message = "Both password fields should represent same text.";

            s.gui->popup_modal = shared_ptr<GUI::PopupModal>(new GUI::PopupModal("msg_login", title, message, 0));
        }
    }
    ImGui::PopID();

    btn1.setTextureRect(sf::IntRect(0, 40, 120, 40));
    btn2.setTextureRect(sf::IntRect(120, 40, 120, 40));

    // button: Cancel
    ImGui::SetCursorPos(ImVec2(472, 416));
    ImGui::PushID(9);
    if(ImGui::ImageAnimButton(btn1, btn2))
    {
        this->SetState(State::StartScreen);
    }
    ImGui::PopID();

    ImGui::PopStyleColor(2);
    ImGui::PopFont();

    if(this->create_account.get())
    {
        if(this->create_account->approved && this->create_account->creation_clock.getElapsedTime().asSeconds() >= 1.0)
        {
            std::vector<TextField> cont = this->text_fields;
            s.eoclient->AccountCreate(cont[0].GetText(), cont[1].GetText(), cont[3].GetText(), cont[4].GetText(), cont[5].GetText());
            this->create_account.reset();
        }

        if(this->popup_modal.get())
        {
            if(!this->popup_modal->open)
            {
                // account creation process starts from the beginning and the account needs to be approved again
                this->create_account->approved = false;
            }
        }
    }
}

void GUI::CharacterList()
{
    S &s = S::GetInstance();

    if(!this->bg_avatar.get())
    {
        int rand_num = s.rand_gen.RandInt(1, 8);

        int tex_id = 60 + rand_num;
        shared_ptr<sf::Texture> texture = s.gfx_loader->LoadTexture(1, tex_id);
        this->bg_avatar = shared_ptr<sf::Sprite>(new sf::Sprite(*texture));
        sf::FloatRect gb = this->bg_avatar->getGlobalBounds();
        this->bg_avatar->setPosition(10, 480 - gb.height - 10);
    }

    this->window.draw(*this->bg_avatar);

    shared_ptr<sf::Texture> tex1 = s.gfx_loader->LoadTexture(1, 24);

    ImGui::SetCursorPos(ImVec2(640.0f - tex1->getSize().x - 1.0f, -1.0f));
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, ImVec4(239.0f, 222.0f, 189.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, ImVec4(239.0f, 222.0f, 189.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, ImVec4(239.0f, 222.0f, 189.0f, 0.0f));
    if(ImGui::ImageAnimButton(sf::Sprite(*tex1, sf::IntRect(0, 0, 51, 53)),
                                               sf::Sprite(*tex1, sf::IntRect(0, 53, 51, 53))))
    {
        s.eoclient->Disconnect();
        this->SetState(GUI::State::StartScreen);
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();

    tex1 = s.gfx_loader->LoadTexture(1, 11, false);
    int pressed_id = -1;
    bool pressed_login = false;
    bool pressed_delete = false;
    int button_id = 0;
    for(std::size_t i = 0; i < 3; ++i)
    {
        int y = 36 + i * tex1->getSize().y + i;
        ImGui::SetCursorPos(ImVec2(325.0f, y));
        ImGui::PushID(i);
        ImGui::Image(*tex1);
        ImGui::PopID();
        shared_ptr<sf::Texture> tex2 = s.gfx_loader->LoadTexture(1, 15);
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(325.0f + 166.0f, y + 29.0f));
        shared_ptr<Character> character = s.eoclient->GetAccountCharacter(i);
        if(character.get())
        {
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
            ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImColor(180, 160, 140, 255));
            std::string name_upper = character->name;
            char lower = character->name[0];
            name_upper[0] = toupper(lower);;
            ImGui::Text(name_upper.c_str());
            ImGui::PopFont();
            ImGui::PopStyleColor();
        }
        ImGui::SetCursorPos(ImVec2(325.0f + 160.0f, y + 55.0f));
        ImGui::PushID(button_id++);
        if(ImGui::ImageAnimButton(sf::Sprite(*tex2, sf::IntRect(0, 2 * 29, 91, 29)),
                                               sf::Sprite(*tex2, sf::IntRect(91, 2 * 29, 91, 29))))
        {
            pressed_login = true;
        }
        ImGui::PopID();
        ImGui::SetCursorPos(ImVec2(325.0f + 160.0f, y + 84.0f + 1.0f + ImGui::GetStyle().FramePadding.y));
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
            s.eoclient->SelectCharacter(character->id);
            s.character = character;
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
    ImGui::SetCursorPos(ImVec2(324.0f, 416.0f));
    if(ImGui::ImageAnimButton(sf::Sprite(*tex1, sf::IntRect(0, 0, 120, 40)),
                                               sf::Sprite(*tex1, sf::IntRect(120, 0, 120, 40))))
    {
       // create character button pressed
    }

    ImGui::SetCursorPos(ImVec2(324.0f + 120.0f + 1.0f + ImGui::GetStyle().FramePadding.y, 416.0f));
    if(ImGui::ImageAnimButton(sf::Sprite(*tex1, sf::IntRect(0, 3 * 40, 120, 40)),
                                               sf::Sprite(*tex1, sf::IntRect(120, 3 * 40, 120, 40))))
    {
       // change password button pressed
    }
}

void GUI::GameWindow()
{
    // main game screen!

    S &s = S::GetInstance();

    if(!this->bg.get())
    {
        shared_ptr<sf::Texture> texture = s.gfx_loader->LoadTexture(2, 1);
        this->bg = shared_ptr<sf::Sprite>(new sf::Sprite(*texture.get()));
    }

    shared_ptr<sf::Texture> tex1 = s.gfx_loader->LoadTexture(2, 39);

    ImGui::SetCursorPos(ImVec2(640.0f - tex1->getSize().x - 1.0f, -1.0f));
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, ImVec4(239.0f, 222.0f, 189.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, ImVec4(239.0f, 222.0f, 189.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, ImVec4(239.0f, 222.0f, 189.0f, 0.0f));
    if(ImGui::ImageAnimButton(sf::Sprite(*tex1, sf::IntRect(0, 0, 51, 53)),
                                               sf::Sprite(*tex1, sf::IntRect(0, 53, 51, 53))))
    {
        s.eoclient->Disconnect();
        this->SetState(GUI::State::StartScreen);
        this->bg.reset();
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();

    ImGui::PushItemWidth(460.0f);
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImColor(165, 130, 105, 0));
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImColor(0, 0, 0, 255));
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);

    ImGui::SetCursorPos(ImVec2(118.0f, 310.0f));
    ImGui::PushID(1);
    if(ImGui::InputText("", (char *)this->text_fields[0].text.c_str(), this->text_fields[0].max_len + 1, ImGuiInputTextFlags_EnterReturnsTrue ))
    {
        if(this->text_fields[0].GetText().length() > 0)
        {
            s.eoclient->Talk(this->text_fields[0].GetText());
            this->chat_console.AddMessage(ChatConsole::ChatMessage(s.character->name, this->text_fields[0].GetText()));
            this->text_fields[0].text.clear();
        }

        ImGui::SetKeyboardFocusHere();
    }
    ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0, 0, 0, 255));
    ImGui::PopID();

    ImGui::PopItemWidth();

    ImGui::SetCursorPos(ImVec2(102.0f, 330.0f));

    this->chat_console.Draw();

    ImGui::PopStyleColor(2);
    ImGui::PopFont();
}
