// Endless Online Awaken v0.0.1

#include "handlers.hpp"
#include "../singleton.hpp"
#include "../const/account.hpp"
#include "../character.hpp"

#include <memory>

using std::shared_ptr;

void Login_Reply(PacketReader reader)
{
    S &s = S::GetInstance();

    LoginReply reply = static_cast<LoginReply>(reader.GetShort());

    if(reply == LoginReply::OK)
    {
        puts("LOGIN OK!");

        int characters = reader.GetChar();
        reader.GetByte();
        reader.GetByte();

        for(int i = 0; i < characters; ++i)
        {
            Character character;
            character.name = reader.GetBreakString();
            character.id = reader.GetInt();
            character.level = reader.GetChar();
            character.gender = static_cast<Gender>(reader.GetChar());
            character.hair_style = reader.GetChar(); // hair style
            character.hair_color = reader.GetChar(); // hair color
            character.race = static_cast<Skin>(reader.GetChar()); // race
            character.admin_level = static_cast<AdminLevel>(reader.GetChar()); // admin
            for(int j = 0; j < 5; ++j)
            {
                reader.GetShort(); // paperdoll
            }
            reader.GetByte(); // break

            s.eoclient.account.characters.push_back(character);
        }

        s.eoclient.SetState(EOClient::State::LoggedIn);
        s.gui.SetState(GUI::State::CharacterList);
    }
    else
    {
        puts("Login failed");

        std::string title = "Could not login";
        std::string message = "";

        if(reply == LoginReply::WrongUser)
        {
            message = "The given username is incorrect.";
        }
        else if(reply == LoginReply::WrongUserPass)
        {
            message = "The given password is incorrect.";
        }
        else if(reply == LoginReply::LoggedIn)
        {
            message = "This account is already logged in.";
        }
        else if(reply == LoginReply::Busy)
        {
            message = "Sorry, the game server is currently busy.";
        }

        s.gui.popup_modal = shared_ptr<GUI::PopupModal>(new GUI::PopupModal("msg_login", title, message, 0));
    }
}
