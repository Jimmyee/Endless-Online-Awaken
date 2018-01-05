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
    static std::map<unsigned char, bool> keys;
    static Direction direction;
    static bool rewalk;

    InputHandler();

    void ProcessEvent(ALLEGRO_EVENT event);
    void Process();

    void CharacterMovement();
};

#endif // INPUT_HANDLER_HPP_INCLUDED
