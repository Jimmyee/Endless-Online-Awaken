// Endless Online Awaken v0.0.1

#include "handlers.hpp"
#include "../singleton.hpp"

// NPC appears in range
void Appear_Reply(PacketReader reader)
{
    reader.GetChar(); // 0
    reader.GetChar(); // 255
    unsigned char index = reader.GetChar();
    // read the rest
}
