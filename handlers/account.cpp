// Endless Online Awaken v0.0.1

#include "handlers.hpp"
#include "../singleton.hpp"
#include "../packet.hpp"
#include "../const/account.hpp"
#include "../gui.hpp"

void Account_Reply(PacketReader reader)
{
    S &s = S::GetInstance();

    AccountReply reply = static_cast<AccountReply>(reader.GetShort());
    std::string reply_str = reader.GetEndString();

    if(reply_str == "OK")
    {
        if(reply == AccountReply::Continue)
        {
            puts("OK: Account name approved");
            s.gui.create_account->approved = true;
            s.gui.create_account->creation_clock.restart();

        }
        else if(reply == AccountReply::Created)
        {
            puts("OK: Account created");
            s.gui.SetState(GUI::State::StartScreen);
            std::string title =  "Account created!";
            std::string message = "Type your new username and password and log in to the game.";
            s.gui.popup_modal = shared_ptr<GUI::PopupModal>(new GUI::PopupModal("msg_create_acc", title, message, 0));
        }
    }
    else if(reply_str == "NO")
    {
        std::string title =  "Could not create account";
        std::string message = "The reason is unknown.";

        if(reply == AccountReply::Exists)
        {
            puts("NO: Such account already exists");
            message = "Account with given name already exists.";
        }
        else if(reply == AccountReply::NotApproved)
        {
            puts("NO: account not approved");
            message = "Account has not been approved.";
        }

        s.gui.popup_modal = shared_ptr<GUI::PopupModal>(new GUI::PopupModal("msg_create_acc", title, message, 0));
    }
}
