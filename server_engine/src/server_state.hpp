// Endless Online Awaken

#ifndef SERVER_STATE_HPP_INCLUDED
#define SERVER_STATE_HPP_INCLUDED

// (monostate)
class ServerState
{
public:
    enum class State
    {
        Exit,
        Run
    };

private:
    static bool initialized_;
    static State state;

public:
    ServerState();
    State Get();
    void Set(State state);
};

#endif // SERVER_STATE_HPP_INCLUDED
