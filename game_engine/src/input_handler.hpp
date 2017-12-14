// Endless Online Awaken

#ifndef INPUT_HANDLER_HPP_INCLUDED
#define INPUT_HANDLER_HPP_INCLUDED

#include "const/entity.hpp"

#include <allegro5/allegro.h>
#include <map>

// (monostate)
class InputHandler
{
private:
    static bool initialized_;

public:
    static bool walk;
    static Direction direction;
    static std::map<unsigned char, bool> keys;

    InputHandler();

    void ProcessEvent(ALLEGRO_EVENT event);
    void Process();

    void CharacterDirection(ALLEGRO_KEYBOARD_EVENT event);
};

#endif // INPUT_HANDLER_HPP_INCLUDED
