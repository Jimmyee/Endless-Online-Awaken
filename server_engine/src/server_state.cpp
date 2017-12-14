// Endless Online Awaken

#include "server_state.hpp"

bool ServerState::initialized_ = false;
ServerState::State ServerState::state = ServerState::State::Run;

ServerState::ServerState()
{
    if(!this->initialized_)
    {
        this->initialized_ = true;
    }
}

ServerState::State ServerState::Get()
{
    return this->state;
}

void ServerState::Set(State state)
{
    this->state = state;
}
