// Endless Online Awaken v0.0.1

#include "handlers.hpp"
#include "../singleton.hpp"

void Talk_Player(PacketReader reader)
{
    S &s = S::GetInstance();

    short gameworld_id = reader.GetShort();
    std::string message = reader.GetEndString();

    std::string name = "[Unknown]";
    shared_ptr<Character> character = s.map->GetCharacter(gameworld_id);
    if(character.get())
    {
        name = character->name;
    }

    s.gui->chat_console.AddMessage(GUI::ChatConsole::ChatMessage(name, message));
}
