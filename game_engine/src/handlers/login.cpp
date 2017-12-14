// Endless Online Awaken

#include "login.hpp"

#include "h_character.hpp"
#include "../client.hpp"
#include "../gui.hpp"
#include "../map.hpp"
#include "../character.hpp"

#include <iostream>

namespace PacketHandlers::HLogin
{
    void Main(sf::Packet packet, std::array<intptr_t, 4> data_ptr)
    {
        Client client;
        unsigned char sub_id = 0;
        unsigned char answer = 0;
        std::string message = "";

        packet >> sub_id;
        packet >> answer;


        if(sub_id == 1)
        {
            // reply for login request
            packet >> message;

            if(answer)
            {
                Client client;
                client.state = Client::State::Logged_in;
                client.packet_handler.Register(PacketID::Character, PacketHandlers::HCharacter::Main, data_ptr);
                GUI().SetState(GUI::State::CharacterList);

                std::size_t characters = 0;
                packet >> characters;

                for(std::size_t i = 0; i < characters; ++i)
                {
                    std::string name = "";
                    unsigned char gender = 0;

                    packet >> name;
                    packet >> gender;

                    Character character;
                    character.name = name;
                    character.gender = (Gender)gender;

                    client.characters.push_back(character);
                }
            }
            else
            {
                GUI().OpenPopup("Server answer", message);
            }

            std::cout << message << std::endl;
        }
        if(sub_id == 2)
        {
            // reply for character selection
        }
    }
}
