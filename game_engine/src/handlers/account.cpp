// Endless Online Awaken

#include "account.hpp"

#include "../client.hpp"
#include "../gui.hpp"

#include <iostream>
#include <imgui.h>
#include "../imgui_impl_a5.h"

namespace PacketHandlers::HAccount
{

    void Main(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        unsigned char sub_id = 0;

        packet >> sub_id;

        if(sub_id == 1) // create account reply
        {
            Create(packet, data_ptr);
        }
        if(sub_id == 2) // delete account reply
        {
            Delete(packet, data_ptr);
        }
    }

    void Create(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {
        GUI gui;
        unsigned char answer = 0;
        std::string message;

        packet >> answer;
        packet >> message;

        if(answer == 1)
        {
            std::cout << "Account created!" << std::endl;

            gui.SetState(GUI::State::MainMenu);
            gui.OpenPopup("Account created");
        }
        if(answer == 0)
        {
            gui.OpenPopup("Server answer", message);
        }
    }

    void Delete(sf::Packet &packet, std::array<intptr_t, 4> data_ptr)
    {

    }

}
