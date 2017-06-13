#include "handlers.hpp"
#include "../singleton.hpp"
#include "../eoclient.hpp"
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
            reader.GetChar(); // level
            reader.GetChar(); // gender
            reader.GetChar(); // hair style
            reader.GetChar(); // hair color
            reader.GetChar(); // race
            reader.GetChar(); // admin
            for(int j = 0; j < 5; ++j)
            {
                reader.GetShort(); // paperdoll
            }
            reader.GetByte(); // break

            char lower = character.name[0];
            character.name[0] = toupper(lower);
            s.eoclient->account_characters.push_back(shared_ptr<Character>(new Character(character)));
        }

        s.eoclient->SetState(EOClient::State::LoggedIn);
        s.gui->SetState(GUI::State::CharacterList);
    }
    else
    {
        puts("Login failed");
    }
}
