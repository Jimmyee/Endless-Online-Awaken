#include "handlers.hpp"
#include "../singleton.hpp"

void Trade_Request(PacketReader reader)
{
    reader.GetChar(); // ?
    short gameworld_id = reader.GetShort();
    std::string name = reader.GetEndString();
}

void Trade_Open(PacketReader reader)
{
    S &s = S::GetInstance();

    short gameworld_id = reader.GetShort();
    std::string name = reader.GetBreakString();
    short my_gameworld_id = reader.GetShort();
    std::string my_name = reader.GetBreakString();

    s.eprocessor.trade = std::shared_ptr<EventProcessor::Trade>(new EventProcessor::Trade(gameworld_id));
}

void Trade_Close(PacketReader reader) // other player closed trade
{
    short gameworld_id = reader.GetShort();

    S::GetInstance().eprocessor.trade.reset();
}

void Trade_Reply(PacketReader reader) // update of trade items
{
    S &s = S::GetInstance();

    if(!s.eprocessor.trade.get()) return;

    short gameworld_id = reader.GetShort();

    s.eprocessor.trade->player_items.clear();
    s.eprocessor.trade->victim_items.clear();
    while(reader.PeekByte() != 255)
    {
        short item_id = reader.GetShort();
        int item_amount = reader.GetInt();

        if(gameworld_id == s.character.gameworld_id)
            s.eprocessor.trade->player_items.push_back(std::make_pair(item_id, item_amount));
        else
            s.eprocessor.trade->victim_items.push_back(std::make_pair(item_id, item_amount));
    }
    reader.GetByte(); // 255

    gameworld_id = reader.GetShort();
    while(reader.PeekByte() != 255)
    {
        short item_id = reader.GetShort();
        int item_amount = reader.GetInt();

        if(gameworld_id == s.character.gameworld_id)
            s.eprocessor.trade->player_items.push_back(std::make_pair(item_id, item_amount));
        else
            s.eprocessor.trade->victim_items.push_back(std::make_pair(item_id, item_amount));
    }
    reader.GetByte(); // 255
}

void Trade_Spec(PacketReader reader) // trade agree status update
{
    S &s = S::GetInstance();

    if(!s.eprocessor.trade.get()) return;

    unsigned char agree = reader.GetChar();
    s.eprocessor.trade->player_accepted = agree;
}

void Trade_Agree(PacketReader reader) // trade agree status update
{
    S &s = S::GetInstance();

    if(!s.eprocessor.trade.get()) return;

    short victim_gameworld_id = reader.GetShort();
    unsigned char agree = reader.GetChar();

    s.eprocessor.trade->victim_accepted = agree;
}

void Trade_Use(PacketReader reader) // trade finished
{
    S &s = S::GetInstance();

    short gameworld_id = reader.GetShort();

    s.eprocessor.trade->player_items.clear();
    s.eprocessor.trade->victim_items.clear();
    while(reader.PeekByte() != 255)
    {
        short item_id = reader.GetShort();
        int item_amount = reader.GetInt();

        if(gameworld_id == s.character.gameworld_id)
            s.eprocessor.trade->player_items.push_back(std::make_pair(item_id, item_amount));
        else
            s.eprocessor.trade->victim_items.push_back(std::make_pair(item_id, item_amount));
    }
    reader.GetByte(); // 255

    gameworld_id = reader.GetShort();
    while(reader.PeekByte() != 255)
    {
        short item_id = reader.GetShort();
        int item_amount = reader.GetInt();

        if(gameworld_id == s.character.gameworld_id)
            s.eprocessor.trade->player_items.push_back(std::make_pair(item_id, item_amount));
        else
            s.eprocessor.trade->victim_items.push_back(std::make_pair(item_id, item_amount));
    }
    reader.GetByte(); // 255

    for(unsigned int i = 0; i < s.eprocessor.trade->player_items.size(); ++i)
    {
        s.inventory.DelItem(s.eprocessor.trade->player_items[i].first, s.eprocessor.trade->player_items[i].second);
    }

    for(unsigned int i = 0; i < s.eprocessor.trade->victim_items.size(); ++i)
    {
        s.inventory.AddItem(s.eprocessor.trade->victim_items[i].first, s.eprocessor.trade->victim_items[i].second);
    }

    s.eprocessor.trade.reset();
}
