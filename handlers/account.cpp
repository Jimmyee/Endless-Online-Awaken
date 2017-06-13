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
            s.gui->create_account->approved = true;
            s.gui->create_account->creation_clock.restart();

        }
        else if(reply == AccountReply::Created)
        {
            puts("OK: Account created");
            s.gui->SetState(GUI::State::StartScreen);
        }
    }
    else if(reply_str == "NO")
    {
        if(reply == AccountReply::Exists)
        {
            puts("NO: Such account already exists");
        }
        else if(reply == AccountReply::NotApproved)
        {
            puts("NO: account not approved");
        }
    }
}
