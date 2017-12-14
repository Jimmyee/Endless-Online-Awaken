// Endless Online Awaken

#include "game_state.hpp"

bool GameState::initialized_ = false;
GameState::State GameState::state = GameState::State::MainMenu;

GameState::GameState()
{
    if(!this->initialized_)
    {
        this->initialized_ = true;
    }
}

GameState::State GameState::Get()
{
    return this->state;
}

void GameState::Set(State state)
{
    this->state = state;
}
