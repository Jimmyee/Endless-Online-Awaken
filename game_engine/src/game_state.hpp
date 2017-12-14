// Endless Online Awaken

#ifndef STATE_HANDLER_HPP_INCLUDED
#define STATE_HANDLER_HPP_INCLUDED

 // game state handler (monostate)
class GameState
{
public:
    enum class State
    {
        Exit,
        MainMenu,
        Playing
    };

private:
    static bool initialized_;
    static State state;

public:
    GameState();
    State Get();
    void Set(State state);
};

#endif // STATE_HANDLER_HPP_INCLUDED
